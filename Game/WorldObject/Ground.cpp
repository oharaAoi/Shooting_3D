#include "Ground.h"

Ground::Ground(Model* model) { Init(model); }
Ground::~Ground() {
}

void Ground::Init(Model* model) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_.y = -70.0f;
}

void Ground::Update() {
#ifdef _DEBUG
	ImGui::DragFloat3("ground.tranlation", &worldTransform_.translation_.x, 1.0f);
#endif // _DEBUG
	// 計算転送
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(const ViewProjection& viewpro) {
	model_->Draw(worldTransform_, viewpro);
}
