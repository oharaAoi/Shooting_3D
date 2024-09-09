#include "BaseEnemy.h"
#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Init(std::vector<Model*> models) {
	BaseCharacter::Init(models);
	isDead_ = false;
	firstPos_ = worldTransform_.translation_;

	triangleModel_ = ModelLoader::GetInstacne()->GetModel("enemyTriangle");
	triangleTransform_.Initialize();
	triangleTransform_.parent_ = &worldTransform_;
	triangleTransform_.translation_ = { 0,2,0 };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Update() {
	color_.SetColor({ 1,1,1,1 });
	if (hp_ <= 0) {
		isDead_ = true;
	}

	if (enemyType_ != EnemyType::Type_Boss) {
		if (CheckWorldRange()) {
			isDead_ = true;
		}
	}

	hp_ = std::clamp(static_cast<int>(hp_), 0, 200);

	isPreDiscovery_ = isDiscovery_;
	BaseCharacter::Update();
	triangleTransform_.UpdateMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Draw(const ViewProjection& viewProjection) const {
	BaseCharacter::Draw(viewProjection);

	if (isDiscovery_) {
		triangleModel_->Draw(triangleTransform_, viewProjection);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 BaseEnemy::GetWorldPosition() const {
	return Vector3();
}

Vector3 BaseEnemy::GetScreenPosition(const ViewProjection& viewProjection) const {
	Vector3 result{};
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュープロの算出
	Matrix4x4 matView = viewProjection.matView * viewProjection.matProjection * matViewport;
	// スクリーン座標を出す
	result = Transform(GetWorldPosition(), matView);

	return result;
}

Vector3 BaseEnemy::GetScreenPosition(const Vector3& posWorld, const ViewProjection& viewProjection) const {
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// 行列の合成
	Matrix4x4 matViewProjection = viewProjection.matView * viewProjection.matProjection * matViewport;
	// ワールド→スクリーン座標変換
	return Transform(posWorld, matViewProjection);
}

void BaseEnemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

bool BaseEnemy::CheckWorldRange() {
	Vector3 size = { kWorldSize.x + 10, kWorldSize.y + 10, kWorldSize.z + 10 };

	if (worldTransform_.translation_.x < -(size.x / 2.0f) || worldTransform_.translation_.x > (size.x / 2.0f)) {
		return true;
	}

	if (worldTransform_.translation_.y < -(size.y / 2.0f) || worldTransform_.translation_.y >(size.y / 2.0f)) {
		return true;
	}

	if (worldTransform_.translation_.z < -(size.z / 2.0f) || worldTransform_.translation_.z >(size.z / 2.0f)) {
		return true;
	}

	return false;
}

void BaseEnemy::ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior) {
	behaviorState_ = std::move(behavior);
}

void BaseEnemy::HitedEffect() {
	color_.SetColor({ 1,0,0,1 });
	color_.TransferMatrix();
}

void BaseEnemy::ImGuiSetTranslation() {
#ifdef _DEBUG
	ImGui::DragFloat3("Enemy", &worldTransform_.translation_.x, 0.01f);
#endif // _DEBUG
}
