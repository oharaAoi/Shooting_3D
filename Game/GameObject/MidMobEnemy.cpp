#include "MidMobEnemy.h"

MidMobEnemy::MidMobEnemy(std::vector<Model*> models, const Vector3& pos) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBoss));
	worldTransform_.translation_ = pos;
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

	hp_ = 10;
	radius_ = 2.0f;
	isDead_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MidMobEnemy::Update() {
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

// ------------------- 攻撃する ------------------- //

void MidMobEnemy::Attack() {
}

// ------------------- 状態遷移を実際に行う ------------------- //

void MidMobEnemy::ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior) {
	behaviorState_ = std::move(behavior);
}

// ------------------- 状態遷移のリクエストがあるかを確認する ------------------- //

void MidMobEnemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case MidEnemyBehavior::kRoot:
			ChangeBehavior(std::make_unique<MidEnemyRootState>(this));
			break;

		case MidEnemyBehavior::kAttack:
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
