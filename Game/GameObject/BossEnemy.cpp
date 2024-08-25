#include "BossEnemy.h"

BossEnemy::BossEnemy(std::vector<Model*> models, const Vector3& pos) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kBoss));
	worldTransform_.translation_ = pos;
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

	worldTransform_.scale_ = { 6.0f, 6.0f, 6.0f };
	worldTransform_.translation_ = { 0.0f,0.0f,50.0f };
	worldTransform_.rotation_ = { 0.0f,3.1f,0.0f };

	behaviorRequest_ = EnemyBehavior::kRoot;
	ChangeBehavior(std::make_unique<BossRootState>(this));

	hp_ = 50;
	radius_ = 2.0f;
	isDead_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Update() {
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

void BossEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseEnemy::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 攻撃を行う関数 ------------------- //

void BossEnemy::Attack() {
}

// ------------------- 状態遷移のリクエストがあるかを確認する ------------------- //

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
	}
}

// ------------------- ImGuiを編集する関数 ------------------- //

void BossEnemy::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("Boss");
	ImGui::DragScalar("hp", ImGuiDataType_U32, &hp_);
	ImGui::Separator();
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
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
