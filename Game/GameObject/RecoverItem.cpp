#include "RecoverItem.h"

RecoverItem::RecoverItem(Model* model, const Vector3& pos) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kRecoverItem));
	Init(model, pos);
}

RecoverItem::~RecoverItem() {
}

void RecoverItem::Init(Model* model, const Vector3& pos) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	radius_ = 2.0f;

	isDead_ = false;
}

void RecoverItem::Update() {
	worldTransform_.UpdateMatrix();
}

void RecoverItem::Draw(const ViewProjection& viewpro) {
	model_->Draw(worldTransform_, viewpro);
}

Vector3 RecoverItem::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

void RecoverItem::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		isDead_ = true;
	}
}
