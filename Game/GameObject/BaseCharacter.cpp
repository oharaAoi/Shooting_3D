#include "BaseCharacter.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseCharacter::Init(std::vector<Model*> models) {
	models_ = models;
	worldTransform_.Initialize();

	// モデルのコンテナからキーを取得しそれに応じたtransformを作成
	for (uint32_t oi = 0; oi < models_.size(); oi++) {
		worldTransforms_.emplace_back(std::move(WorldTransform{}));
		worldTransforms_[oi].Initialize();
	}
	obb_.size = { 1,1,1 };

	//Collider::Inti();

	color_.Initialize();
	color_.SetColor({ 1,1,1,1 });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseCharacter::Update() {
	obb_.center = worldTransform_.translation_;
	obb_.MakeOBBAxis(worldTransform_.rotation_);
	worldTransform_.UpdateMatrix();
	color_.TransferMatrix();
	for (uint32_t oi = 0; oi < models_.size(); oi++) {
		worldTransforms_[oi].UpdateMatrix();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseCharacter::Draw(const ViewProjection& viewProjection) const {
	for (uint32_t oi = 0; oi < models_.size(); oi++) {
		models_[oi]->Draw(worldTransforms_[oi], viewProjection, &color_);
	}
}

void BaseCharacter::DrawCollision() {
	DrawOBB(obb_, { 1,1,1,1 });
}

Vector3 BaseCharacter::GetWorldPosition() const {
	return Vector3();
}
