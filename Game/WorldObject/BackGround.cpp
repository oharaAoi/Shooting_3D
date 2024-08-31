#include "BackGround.h"

BackGround::BackGround(Model* model) {
	Init(model);
}

BackGround::~BackGround() {
}

void BackGround::Init(Model* model) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_.z = 0.0f;
}

void BackGround::Update() {
#ifdef _DEBUG
	ImGui::DragFloat3("backGround.tranlation", &worldTransform_.translation_.x, 1.0f);
	ImGui::DragFloat3("backGround.rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("backGround.scale", &worldTransform_.scale_.x, 0.1f);
#endif // _DEBUG
	// 計算転送
	worldTransform_.UpdateMatrix();
}

void BackGround::Draw(const ViewProjection& viewpro) {
	model_->Draw(worldTransform_, viewpro);
}
