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

	hp_ = 50;

	radius_ = 2.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossEnemy::Update() {
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

void BossEnemy::Attack() {
}

void BossEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {

	}
}

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
