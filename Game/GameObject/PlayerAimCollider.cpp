#include "PlayerAimCollider.h"

PlayerAimCollider::PlayerAimCollider(const Vector3& size) {
	Init(size);
}

PlayerAimCollider::~PlayerAimCollider() {
}

void PlayerAimCollider::Init(const Vector3& size) {
	obb_.size = size;
}

void PlayerAimCollider::Update(const Vector3& center, const Vector3& rotation) {
	obb_.center = center;
	obb_.MakeOBBAxis(rotation);
}

void PlayerAimCollider::Draw() {
	DrawOBB(obb_, { 1,1,1,1 });
}
