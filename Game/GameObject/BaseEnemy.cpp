#include "BaseEnemy.h"
#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Init(std::vector<Model*> models) {
	BaseCharacter::Init(models);
	isDead_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseEnemy::Update() {
	if (hp_ <= 0) {
		isDead_ = true;
	}
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

Vector3 BaseEnemy::GetScreenPosition(const ViewProjection& viewProjection) {
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

void BaseEnemy::ImGuiSetTranslation() {
	ImGui::DragFloat3("Enemy", &worldTransform_.translation_.x, 0.01f);
}
