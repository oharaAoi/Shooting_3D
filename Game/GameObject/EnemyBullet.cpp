#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) {
	Init(model, pos, velocity, rotation, parent);
}

EnemyBullet::~EnemyBullet() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) {
	BaseBullet::Init(model, pos, velocity, rotation, parent);

	// velocityにplayerの座標が入っているため,playerの方向へのVelocityに変更する
	Vector3 direction = velocity_ - worldTransform_.translation_;
	velocity_ = Normalize(direction) * kBulletSpeed_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyBullet::Update() {
	BaseBullet::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyBullet::Draw(const ViewProjection& viewProjection) const {
	BaseBullet::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 衝突時に行う処理 ------------------- //

void EnemyBullet::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		isDead_ = true;
	}
}

// ------------------- world空間上での位置を取得する ------------------- //

Vector3 EnemyBullet::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
