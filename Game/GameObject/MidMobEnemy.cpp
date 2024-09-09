#include "MidMobEnemy.h"
#include "GameScene.h"

MidMobEnemy::MidMobEnemy(std::vector<Model*> models, const Vector3& pos, const Vector3& velocity) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBoss));
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
	Init(models);
}

MidMobEnemy::~MidMobEnemy() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MidMobEnemy::Init(std::vector<Model*> models) {
	BaseEnemy::Init(models);
	enemyType_ = EnemyType::Type_MidEnemy;
	worldTransforms_[MidEnemyParts::MidMob_Body].parent_ = &worldTransform_;
	worldTransforms_[MidEnemyParts::MidMob_LEye].parent_ = &worldTransform_;
	worldTransforms_[MidEnemyParts::MidMob_REye].parent_ = &worldTransform_;

	bulletModel_ = ModelLoader::GetInstacne()->GetModel("bossBullet");

	// ---------------------------------------------
	// ↓ 調整項目
	// ---------------------------------------------
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	const char* groupName = "MidEnemy";
	// グループを追加
	adjustItem->CreateGroup(groupName);
	adjustItem->AddItem(groupName, "body_Translation", worldTransforms_[MidEnemyParts::MidMob_Body].translation_);
	adjustItem->AddItem(groupName, "L_Eye_Translation", worldTransforms_[MidEnemyParts::MidMob_LEye].translation_);
	adjustItem->AddItem(groupName, "R_Eye_Translation", worldTransforms_[MidEnemyParts::MidMob_REye].translation_);

	worldTransforms_[MidEnemyParts::MidMob_Body].translation_ = adjustItem->GetValue<Vector3>(groupName, "body_Translation");
	worldTransforms_[MidEnemyParts::MidMob_LEye].translation_ = adjustItem->GetValue<Vector3>(groupName, "L_Eye_Translation");
	worldTransforms_[MidEnemyParts::MidMob_REye].translation_ = adjustItem->GetValue<Vector3>(groupName, "R_Eye_Translation");

	// ---------------------------------------------
	// ↓ ステート
	// ---------------------------------------------
	behaviorRequest_ = EnemyBehavior::kRoot;
	ChangeBehavior(std::make_unique<MidEnemyRootState>(this));

	// ---------------------------------------------
	// ↓ パラメータ
	// ---------------------------------------------
	hp_ = 3;
	radius_ = 2.0f;
	isDead_ = false;

	animation_.t = 0.0f;
	animation_.amplitudeX = 0.05f;
	animation_.amplitudeY = 0.02f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MidMobEnemy::Update() {
	if (!isPreDiscovery_ && isDiscovery_) {
		behaviorRequest_ = EnemyBehavior::kAttack;
	} else if(!isDiscovery_) {
		behaviorRequest_ = EnemyBehavior::kRoot;
	}

	// 状態の変更のリクエストがあるかを確認する
	CheckBehaviorRequest();
	// 現在の状態を更新する
	behaviorState_->Update();

	BaseEnemy::Update();

	// ImGuiの編集
	EditImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MidMobEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseEnemy::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 通常時の挙動をする ------------------- //

void MidMobEnemy::AnimationGimmick() {
	if (animation_.t > 4 * std::numbers::pi_v<float>) {
		SetBehaviorRequest(EnemyBehavior::kAttack);
		animation_.t = 0.0f;
	}

	Vector2 point;
	Vector3 enemyPos = worldTransform_.translation_;
	animation_.t += 0.1f;
	point.x = (animation_.amplitudeX * std::cos(animation_.t)) + enemyPos.x;
	point.y = (animation_.amplitudeY * std::sin(2 * animation_.t)) + enemyPos.y;

	enemyPos = { point.x, point.y, enemyPos.z };
	worldTransform_.translation_ = enemyPos;
}

// ------------------- 移動を行う関数 ------------------- //

void MidMobEnemy::Move() {
	TurnAoundVelocity();
	AnimationGimmick();
	const float speed = 0.05f;
	if (isDiscovery_) {
		velocity_ = playerPosition_ - worldTransform_.translation_;
	}
	worldTransform_.translation_ += Normalize(velocity_) * speed;
}

// ------------------- 攻撃する ------------------- //

void MidMobEnemy::Attack() {
	Shot();
}

// ------------------- リストに弾を追加する ------------------- //

void MidMobEnemy::Shot() {
	gameScene_->AddEnemyBullet(std::move(std::make_unique<EnemyBullet>(
		bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, true
	)));
}

// ------------------- 進行方向に回転する ------------------- //

void MidMobEnemy::TurnAoundVelocity() {
	// Y軸周りで回転させる角度を求める
	float targetAngleY = std::atan2f(velocity_.x, velocity_.z);
	// X軸周りで回転させる角度を求める
	float xzLenght = Length({ velocity_.x, 0, velocity_.z });
	float targetAngleX = std::atan2f(-velocity_.y, xzLenght);
	// 振り向かせる
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngleY, 0.03f);
	worldTransform_.rotation_.x = LerpShortAngle(worldTransform_.rotation_.x, targetAngleX, 0.03f);
}

// ------------------- 状態遷移のリクエストがあるかを確認する ------------------- //

void MidMobEnemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case EnemyBehavior::kRoot:
			ChangeBehavior(std::make_unique<MidEnemyRootState>(this));
			break;

		case EnemyBehavior::kAttack:
			ChangeBehavior(std::make_unique<MidEnemyAttackState>(this));
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

// ------------------- 攻撃を食らった時の演出 ------------------- //

void MidMobEnemy::HitedEffect() {
	BaseEnemy::HitedEffect();
}

// ------------------- 衝突時に行う関数 ------------------- //

void MidMobEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		hp_--;
		hp_ -= other->GetCollisionCount();
		gameScene_->AddEmissionEffect(other->GetTranslation(), 60, 5);

		if (other->GetCollisionCount() > 2) {
			gameScene_->AddRecoverItem(worldTransform_.translation_);
		}
	}
}

// ------------------- ImGuiを編集する ------------------- //

void MidMobEnemy::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("midEnemy");
	//ImGui::DragFloat3("body", &worldTransforms_[MidEnemyParts::MidMob_Body].translation_.x, 0.1f);
	ImGui::DragFloat3("leftEye", &worldTransforms_[MidEnemyParts::MidMob_LEye].translation_.x, 0.1f);
	ImGui::DragFloat3("rightEye", &worldTransforms_[MidEnemyParts::MidMob_REye].translation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 MidMobEnemy::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
