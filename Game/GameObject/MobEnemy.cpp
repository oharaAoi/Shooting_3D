#include "MobEnemy.h"
#include "GameScene.h"

MobEnemy::MobEnemy(std::vector<Model*> models, const Vector3& pos) {
	enemyId_ = mobEnemyNum;
	mobEnemyNum++;
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	worldTransform_.translation_ = pos;
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

	behaviorRequest_ = MobEnemyBehavior::kAttack;
	ChangeBehavior(std::make_unique<MobRootState>(this));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobEnemy::Update() {
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
}

// ------------------- 攻撃に関する関数 ------------------- //

void MobEnemy::Attack() {
	Shot();
}

// ------------------- 弾を撃つ ------------------- //

void MobEnemy::Shot() {
	gameScene_->AddEnemyBullet(std::move(std::make_unique<EnemyBullet>(
		bulletModel_, worldTransform_.translation_, playerPosition_, worldTransform_.rotation_, worldTransform_.parent_
	)));
}

// ------------------- 状態を遷移させる ------------------- //

void MobEnemy::ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior) {
	behaviorState_ = std::move(behavior);
}

// ------------------- 状態遷移のリクエストがあるかを確認する ------------------- //

void MobEnemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case MobEnemyBehavior::kRoot:
			ChangeBehavior(std::make_unique<MobRootState>(this));
			break;

		case MobEnemyBehavior::kAttack:
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
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////
