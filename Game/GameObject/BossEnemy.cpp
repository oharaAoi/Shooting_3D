#include "BossEnemy.h"
#include "GameScene.h"

BossEnemy::BossEnemy(std::vector<Model*> models, const Vector3& pos, const Vector3& velocity) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBoss));
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
	Init(models);
}

BossEnemy::~BossEnemy() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Init(std::vector<Model*> models) {
	BaseEnemy::Init(models);
	enemyType_ = EnemyType::Type_Mob;
	worldTransforms_[BossParts::Boss_Body].parent_ = &worldTransform_;
	worldTransforms_[BossParts::Boss_LEye].parent_ = &worldTransform_;
	worldTransforms_[BossParts::Boss_REye].parent_ = &worldTransform_;

	// ---------------------------------------------
	// ↓ 調整項目
	// ---------------------------------------------
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	const char* groupName = "Boss";
	// グループを追加
	adjustItem->CreateGroup(groupName);
	adjustItem->AddItem(groupName, "body_Translation", worldTransforms_[BossParts::Boss_Body].translation_);
	adjustItem->AddItem(groupName, "L_Eye_Translation", worldTransforms_[BossParts::Boss_LEye].translation_);
	adjustItem->AddItem(groupName, "R_Eye_Translation", worldTransforms_[BossParts::Boss_REye].translation_);
	adjustItem->AddItem(groupName, "OBB_Size", obb_.size);

	worldTransforms_[BossParts::Boss_Body].translation_ = adjustItem->GetValue<Vector3>(groupName, "body_Translation");
	worldTransforms_[BossParts::Boss_LEye].translation_ = adjustItem->GetValue<Vector3>(groupName, "L_Eye_Translation");
	worldTransforms_[BossParts::Boss_REye].translation_ = adjustItem->GetValue<Vector3>(groupName, "R_Eye_Translation");
	obb_.size = adjustItem->GetValue<Vector3>(groupName, "OBB_Size");

	bulletModel_ = ModelLoader::GetInstacne()->GetModel("bossBullet");
	
	worldTransform_.rotation_ = { 0.0f,3.1f,0.0f };

	behaviorRequest_ = EnemyBehavior::kRoot;
	ChangeBehavior(std::make_unique<BossRootState>(this));

	attackType_ = BossAttackType::Rush_Attack;

	// --------------------------------------
	// パラメータの初期化
	// --------------------------------------
	hp_ = 150;
	radius_ = 7.0f;
	isDead_ = false;
	forwardDot_ = 0;

	floating_.parameter = 0;
	floating_.period = 100;
	floating_.amplitude = 0.1f;

	rushChargeTimeCount_ = 0;
	rushChargeTime_ = 80;
	rushTime_ = 80;
	stiffnessTime_ = 0;

	firstHp_ = hp_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Update() {

	if (!isRush_) {
		TurnAroundPlayer();
	}

	// 状態の変更のリクエストがあるかを確認する
	CheckBehaviorRequest();
	
	// 突進攻撃中であれば
	if (isRushAttack_) {
		RushAttack();
	} else {
		// 現在の状態を更新する
		behaviorState_->Update();
		worldTransform_.translation_ += Normalize(playerPosition_ - worldTransform_.translation_) * 0.05f;
	}

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -(kWorldSize.x / 2.0f), (kWorldSize.x / 2.0f));
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -(kWorldSize.y / 2.0f), (kWorldSize.y / 2.0f));
	worldTransform_.translation_.z = std::clamp(worldTransform_.translation_.z, -(kWorldSize.z / 2.0f), (kWorldSize.z / 2.0f));

	BaseEnemy::Update();
	// ImGuiの編集
	EditImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseEnemy::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 移動する関数 ------------------- //

void BossEnemy::Move() {
	FloatingGimmick();
}

// ------------------- 攻撃を行う関数 ------------------- //

void BossEnemy::Attack() {
	switch (attackType_) {
	case Normal_Attack:
		NormalShot();
		break;
	case Homing_Attack:
		HomingShot();
		break;
	case TripleHoming_Attack:
		TripleHomingShot();
		break;
	case Rush_Attack:
		isRushAttack_ = true;
		break;
	}
}

// ------------------- 通常の弾を発射する ------------------- //

void BossEnemy::NormalShot() {
	gameScene_->AddBossBullet(
		std::move(std::make_unique<BossBullet>(
			bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, BulletType::Normal_Bullet
	)));
}

// ------------------- 追従する弾を発射する ------------------- //

void BossEnemy::HomingShot() {
	gameScene_->AddBossBullet(
		std::move(std::make_unique<BossBullet>(
			bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, BulletType::Homing_Bullet
	)));
}

// ------------------- 突進攻撃を行う ------------------- //

void BossEnemy::RushAttack() {
	// 突進前の震え
	if (rushChargeTimeCount_ < rushChargeTime_) {
		rushChargeTimeCount_++;
		// 震える演出
		Vector3 randomPos = RandomVector3(-0.1f, 0.1f);
		worldTransform_.translation_ += randomPos;
		// playerまでの距離
		velocity_ = playerPosition_ - worldTransform_.translation_;
		
		// 突進する処理
	} else if(rushTime_ > 0){
		rushTime_--;
		velocity_ = Normalize(velocity_);
		worldTransform_.translation_ += velocity_;
		isRush_ = true;

		// 突進後の処理
	} else  {
		isRush_ = false;
		const float threshold = 0.99f;
		velocity_ = { 0,0,0 };
		stiffnessTime_++;

		// 向きが同じか判定する処理を追加
		Vector3 forward = Normalize(TransformNormal({ 0,0,1 }, worldTransform_.matWorld_));
		forwardDot_ = Dot(forward, Normalize(playerPosition_ - worldTransform_.translation_));
		// ほぼplayerの方向を向いたら
		if ((forwardDot_ >= threshold) || stiffnessTime_ > 140) {
			isRushAttack_ = false;
			rushTime_ = 80;
			stiffnessTime_ = 0;
			rushChargeTimeCount_ = 0;
			ChangeBehavior(std::make_unique<BossRootState>(this));
		}
	}
}

// ------------------- 3つの追従する弾 ------------------- //

void BossEnemy::TripleHomingShot() {
	Vector3 firstPos[3] = { Vector3{-2.5f, 1, 0.3f} * 5.0f, Vector3{0, 1, 0.3f} * 5.0f, Vector3{2.5f, 1, 0.3f} * 5.0f };
	Vector3 forward = TransformNormal({ 0,0,1 }, worldTransform_.matWorld_);

	for (uint32_t oi = 0; oi < 3; ++oi) {
		firstPos[oi] = worldTransform_.translation_ + (forward * firstPos[oi]);
		std::unique_ptr<BossBullet> bossBullet;
		bossBullet = std::make_unique<BossBullet>(
			bulletModel_, firstPos[oi], playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, BulletType::TripleHoming_Bullet
		);
		bossBullet->SetWaitTime(10 * (oi + 1));
		bossBullet->SetIsFire(false);

		gameScene_->AddBossBullet(std::move(bossBullet));
	}
}

// ------------------- 状態を変更する関数 ------------------- //

void BossEnemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case EnemyBehavior::kRoot:
			ChangeBehavior(std::make_unique<BossRootState>(this));
			break;

		case EnemyBehavior::kAttack:
			ChangeAttackType();
			ChangeBehavior(std::make_unique<BossAttackState>(this));
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

// ------------------- 衝突時に呼ばれる関数 ------------------- //

void BossEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		hp_--;
		hp_ -= other->GetCollisionCount();
		gameScene_->AddEmissionEffect(other->GetTranslation(), 60, 5);
		AudioManager::GetInstacne()->AddPlayList("Audio/enemyHited.wav", false, 0.1f);
	}
}

// ------------------- 攻撃を食らった時の演出 ------------------- //

void BossEnemy::HitedEffect() {
	BaseEnemy::HitedEffect();
}

// ------------------- 浮遊する ------------------- //

void BossEnemy::FloatingGimmick() {
	//----------------------------------------------------------
	// bodyのアニメーション
	//----------------------------------------------------------
	// 1フレームでのパラメータ加算値
	const float step = (2.0f * std::numbers::pi_v<float>) / static_cast<float>(floating_.period);
	// パラメータを1ステップ分加算
	floating_.parameter += step;
	// 2πを超えたら0に戻す
	floating_.parameter = std::fmod(floating_.parameter, 2.0f * std::numbers::pi_v<float>);
	// 座標に反映
	Vector3 translate = worldTransform_.translation_;
	translate.y += std::sin(floating_.parameter) * floating_.amplitude;
	worldTransform_.translation_ = translate;
}

// ------------------- Playerの方向を向く関数 ------------------- //

void BossEnemy::TurnAroundPlayer() {
	Vector3 targetAngleVector = playerPosition_ - worldTransform_.translation_;
	// Y軸周りで回転させる角度を求める
	float targetAngleY = std::atan2f(targetAngleVector.x, targetAngleVector.z);
	// X軸周りで回転させる角度を求める
	float xzLenght = Length({ targetAngleVector.x, 0, targetAngleVector.z });
	float targetAngleX = std::atan2f(-targetAngleVector.y, xzLenght);
	// 振り向かせる
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngleY, 0.03f);
	worldTransform_.rotation_.x = LerpShortAngle(worldTransform_.rotation_.x, targetAngleX, 0.03f);
}

// ------------------- 攻撃の種類を選ぶ ------------------- //

void BossEnemy::ChangeAttackType() {
	uint32_t attackNumber = static_cast<uint32_t>(RandomFloat(0, 4));
	attackType_ = static_cast<BossAttackType>(attackNumber);
}

// ------------------- ImGuiを編集する関数 ------------------- //

void BossEnemy::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("Boss");
	ImGui::DragScalar("hp", ImGuiDataType_U32, &hp_);
	ImGui::Separator();
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("body", &worldTransforms_[BossParts::Boss_Body].translation_.x, 0.1f);
	ImGui::DragFloat3("leftEye", &worldTransforms_[BossParts::Boss_LEye].translation_.x, 0.1f);
	ImGui::DragFloat3("rightEye", &worldTransforms_[BossParts::Boss_REye].translation_.x, 0.1f);
	ImGui::Text("attackType: %d", attackType_);
	ImGui::DragFloat("radius", &radius_, 0.1f);
	ImGui::DragFloat3("obb_Size", &obb_.size.x, 0.1f);

	ImGui::End();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 BossEnemy::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
