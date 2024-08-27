#include "MidMobEnemy.h"
#include "GameScene.h"

MidMobEnemy::MidMobEnemy(std::vector<Model*> models, const std::vector<Vector3>& controlPoint, const float& division) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBoss));
	appearanceControlPoint_ = controlPoint;
	worldTransform_.translation_ = appearanceControlPoint_[2];
	division_ = division;
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

	behaviorRequest_ = EnemyBehavior::kRoot;
	ChangeBehavior(std::make_unique<MidEnemyRootState>(this));

	hp_ = 10;
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
	// 状態の変更のリクエストがあるかを確認する
	CheckBehaviorRequest();
	// 現在の状態を更新する
	//behaviorState_->Update();

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

// ------------------- 攻撃する ------------------- //

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

void MidMobEnemy::Move() {
	AnimationGimmick();
}

void MidMobEnemy::Attack() {
	Shot();
}

// ------------------- 攻撃する ------------------- //

void MidMobEnemy::Shot() {
	gameScene_->AddEnemyBullet(std::move(std::make_unique<EnemyBullet>(
		bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_, true
	)));
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

// ------------------- 衝突時に行う関数 ------------------- //

void MidMobEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {
		hp_--;
	}
}

// ------------------- ImGuiを編集する ------------------- //

void MidMobEnemy::EditImGui() {
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
