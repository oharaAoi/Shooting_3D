#include "BaseBullet.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBullet::Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) {
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rotation;

	// 速度
	velocity_ = velocity;

	// 親を設定する
	worldTransform_.parent_ = parent;

	// -------------------------------------------------
	// ↓ 生存しているかを判断するフラグを設定する
	// -------------------------------------------------
	deathTimer_ = kLifeTime;
	isDead_ = false;

	deathTimer_ = kLifeTime;
	isDead_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBullet::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 移動させる
	/*Matrix4x4 matWorld = Multiply(worldTransform_.matWorld_, worldTransform_.parent_->matWorld_);
	Vector3 velocity = TransformNormal({0,0,0.1f}, matWorld);*/
	worldTransform_.translation_ += velocity_;
	// 行列を更新させる
	worldTransform_.UpdateMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBullet::Draw(const ViewProjection& viewProjection) const {
	model_->Draw(worldTransform_, viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBullet::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

