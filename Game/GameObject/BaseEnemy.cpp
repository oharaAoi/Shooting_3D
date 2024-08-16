#include "BaseEnemy.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Init(std::vector<Model*> models) {
	BaseCharacter::Init(models);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Update() {
	BaseCharacter::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseCharacter::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 BaseEnemy::GetWorldPosition() const {
	return Vector3();
}

void BaseEnemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
