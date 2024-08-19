#include "Reticle.h"

Reticle::Reticle() { Init(); }
Reticle::~Reticle() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Init() {
	// --------------------------------------
	// 3Dのレティクル
	// --------------------------------------
	model_ = ModelLoader::GetInstacne()->GetModel("cube");
	worldTransform3D_.Initialize();

	// --------------------------------------
	// 2Dのレティクル
	// --------------------------------------
	textureReticle_ = TextureManager::Load("reticle.png");
	Vector2 spriteReticlePos = { 640, 320 };
	// spriteの生成
	Sprite* sprite = Sprite::Create(textureReticle_, spriteReticlePos, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	sprite2DReticle_ = std::unique_ptr<Sprite>(sprite);

	cameraToReticle_ = 50;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Update(const WorldTransform& worldTransform, const ViewProjection& viewProjection) {
	// 3Dレティクルのworld座標を計算
	Calculate3DReticleWorldPos(worldTransform);

	// 3Dレティクルの位置をScreen上からworld上の位置を求める
	ScreenToWorldOf3DReticle(viewProjection);

	matWorld_ = Multiply(worldTransform3D_.matWorld_, worldTransform.parent_->matWorld_);

	worldTransform3D_.UpdateMatrix();

	ImGui::Begin("Reticle");
	ImGui::SliderFloat("cameraTo3DReticle", &cameraToReticle_, 0, 1000);
	ImGui::DragFloat3("worldTransform3D.translation", &worldTransform3D_.translation_.x);
	Vector3 pos = Get3DReticleWorldPos();
	ImGui::DragFloat3("3DReticleWorldPos", &pos.x);
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform3D_, viewProjection);
}

void Reticle::Draw2DReticle() {
	sprite2DReticle_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 3Dレティクルのworld座標を計算 ------------------- //

void Reticle::Calculate3DReticleWorldPos(const WorldTransform& worldTransform) {
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット
	Vector3 offset = { 0, 0, 1.0f };
	// 自機のワールド行列回転を反映
	//offset = TransformNormal(offset, worldTransform.matWorld_);
	// ベクトルの長さを変える
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3D_.translation_ = worldTransform.translation_ + offset;
}

// ------------------- スクリーン座標からワールド座標を取得して配置する関数(コントローラー) ------------------- //

void Reticle::ScreenToWorldOf3DReticle(const ViewProjection& viewProjection) {

	// joyState------------------------------------------------------------
	Vector2 spritePos = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * 8.0f;
		spritePos.y -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 8.0f;

		// 反映
		sprite2DReticle_->SetPosition(Vector2(static_cast<float>(spritePos.x), static_cast<float>(spritePos.y)));
	}

	spritePos = sprite2DReticle_->GetPosition();

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// viewprojectionViewport合成行列
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// ニアクリップ面上のワールド座標を得る
	Vector3 posNear = Vector3(static_cast<float>(spritePos.x), static_cast<float>(spritePos.y), 0);
	// ファークリップ面上のワールド座標を得る
	Vector3 posFar = Vector3(static_cast<float>(spritePos.x), static_cast<float>(spritePos.y), 1);

	// スクリーンからワールドへ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// レイの方向
	Vector3 direction = posFar - posNear;
	direction = Normalize(direction);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3D_.translation_ = posNear + direction * kDistanceTestObject;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- レティクルのスクリーン座標を取得(3D) ------------------- //

void Reticle::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform3D_.parent_ = parent;
}

Vector3 Reticle::Get3DReticleWorldPos() {
	Vector3 result{};
	// 行列から平行移動分を取得
	result = Transform({ 0, 0, 0 }, worldTransform3D_.matWorld_);

	return result;
}

// ------------------- レティクルのスクリーン座標を設定(2D) ------------------- //

void Reticle::Set3DReticleTo2DReticle(const ViewProjection& viewProjection) {
	Vector3 positionReticle = Get3DReticleWorldPos();
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;
	// ワールドからスクリーン
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// 座標の設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

Vector3 Reticle::GetReticlePosition() {
	// 3DReticleの位置を渡す
	if (is3dReticle_) {


		// 2DReticleの位置を渡す
	} else {

	}

	return Vector3();
}
