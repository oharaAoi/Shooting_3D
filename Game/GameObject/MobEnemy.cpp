#include "MobEnemy.h"
#include "GameScene.h"

MobEnemy::MobEnemy(std::vector<Model*> models, const Vector3& pos, const Vector3& velocity) {
	enemyId_ = mobEnemyNum;
	mobEnemyNum++;
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
	Init(models);
}
MobEnemy::~MobEnemy() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobEnemy::Init(std::vector<Model*> models) {
	BaseEnemy::Init(models);
	enemyType_ = EnemyType::Type_Mob;
	worldTransforms_[EnmeyParts::Enmey_Body].parent_ = &worldTransform_;

	bulletModel_ = ModelLoader::GetInstacne()->GetModel("cube");

	behaviorRequest_ = EnemyBehavior::kRoot;
	ChangeBehavior(std::make_unique<MobRootState>(this));

	hp_ = 1;
	radius_ = 3.0f;

	floating_.parameter = 0;
	floating_.period = 120;
	floating_.amplitude = 0.05f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobEnemy::Update() {
	if (!isPreDiscovery_ && isDiscovery_) {
		behaviorRequest_ = EnemyBehavior::kAttack;
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

void MobEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseEnemy::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Playerの行動関連
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 移動させる関数 ------------------- //

void MobEnemy::Move() {
	TurnAoundVelocity();
	FloatingGimmick();
	const float speed = 0.05f;
	if (isDiscovery_) {
		velocity_ = playerPosition_ - worldTransform_.translation_;
	}
	worldTransform_.translation_ += Normalize(velocity_) * speed;
}

// ------------------- 攻撃に関する関数 ------------------- //

void MobEnemy::Attack() {
	Shot();
}

// ------------------- 弾を撃つ ------------------- //

void MobEnemy::Shot() {
	gameScene_->AddEnemyBullet(std::move(std::make_unique<EnemyBullet>(
		bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, false
	)));
}

// ------------------- 進行方向に回転する ------------------- //

void MobEnemy::TurnAoundVelocity() {
	// Y軸周りで回転させる角度を求める
	float targetAngleY = std::atan2f(velocity_.x, velocity_.z);
	// X軸周りで回転させる角度を求める
	float xzLenght = Length({ velocity_.x, 0, velocity_.z });
	float targetAngleX = std::atan2f(-velocity_.y, xzLenght);
	// 振り向かせる
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngleY, 0.03f);
	worldTransform_.rotation_.x = LerpShortAngle(worldTransform_.rotation_.x, targetAngleX, 0.03f);
}

// ------------------- 浮遊する関数 ------------------- //

void MobEnemy::FloatingGimmick() {
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

// ------------------- 状態遷移のリクエストがあるかを確認する ------------------- //

void MobEnemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case EnemyBehavior::kRoot:
			ChangeBehavior(std::make_unique<MobRootState>(this));
			break;

		case EnemyBehavior::kAttack:
			ChangeBehavior(std::make_unique<MobAttackState>(this));
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

// ------------------- 当たり判定を取った時の関数 ------------------- //

void MobEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		hp_--;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ImGui
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobEnemy::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("MobEnemy");
	std::string label = "Enemy " + std::to_string(enemyId_);
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- world空間での座標を取得する ------------------- //

Vector3 MobEnemy::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

