#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent, const uint32_t& collisionCount) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet));
	collisionCount_ = collisionCount;
	Init(model, pos, velocity, rotation, parent);
}
PlayerBullet::~PlayerBullet() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) {
	BaseBullet::Init(model, pos, velocity, rotation, parent);
	collisionCount_ = 0;
	obb_.size = { 1,1,1 };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::Update() {
	float scaleUp = 1.0f + (0.3f * collisionCount_);
	radius_ = 1.0f + (0.3f * collisionCount_);
	worldTransform_.scale_ = { scaleUp, scaleUp , scaleUp };
	obb_.center = worldTransform_.translation_;
	obb_.MakeOBBAxis(worldTransform_.rotation_);
	BaseBullet::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBullet::Draw(const ViewProjection& viewProjection) const {
	BaseBullet::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 衝突時に行う処理 ------------------- //

void PlayerBullet::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	switch (typeID) {
	case static_cast<uint32_t>(CollisionTypeIdDef::kEnemy):
		collisionCount_++;
		
		break;
	case static_cast<uint32_t>(CollisionTypeIdDef::kBoss):
		isDead_ = true;
		break;
	case static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet):
		collisionCount_++;
		
		break;
	}
}

// ------------------- world空間上での位置を取得する ------------------- //

Vector3 PlayerBullet::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
