#include "Skydome.h"

Skydome::Skydome(Model* model) { Init(model); };
Skydome::~Skydome() {};

//=============================================
// 初期化処理
//=============================================

void Skydome::Init(Model* model) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
}

//=============================================
// 更新処理
//=============================================

void Skydome::Update() {
	// 計算転送
	worldTransform_.UpdateMatrix();
}

//=============================================
// 描画処理
//=============================================

void Skydome::Draw(const ViewProjection& viewpro) {
	model_->Draw(worldTransform_, viewpro);
}
