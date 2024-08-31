#include "Particle.h"

Particle::Particle(Model* model, const Vector3& position, const Vector3& velocity, const uint32_t& lifeTime) {
	Init(model, position, velocity, lifeTime);
}

Particle::~Particle() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Particle::Init(Model* model, const Vector3& position, const Vector3& velocity, const uint32_t& lifeTime) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	float randomRadius = RandomFloat(0.4f, 0.8f);
	worldTransform_.scale_ = Vector3{ randomRadius , randomRadius , randomRadius };
	color_.Initialize();
	color_.SetColor({ 1,1,1,1 });
	lifeTime_ = lifeTime;
	velocity_ = velocity;
	isDead_ = false;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Particle::Update() {
	worldTransform_.translation_ += velocity_ * kSpeed_;

	if (--lifeTime_ <= 0) {
		isDead_ = true;
	}

	color_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Particle::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, &color_);
}
