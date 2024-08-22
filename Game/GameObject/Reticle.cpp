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
	lockOnReticleHandle_ = TextureManager::Load("reticle.png");
	unLockReticleHandle_ = TextureManager::Load("reticle2.png");
	Vector2 spriteReticlePos = { 640, 320 };
	// spriteの生成
	Sprite* sprite = Sprite::Create(lockOnReticleHandle_, spriteReticlePos, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	Sprite* unLockSprite = Sprite::Create(unLockReticleHandle_, spriteReticlePos, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	lockOnReticle_ = std::unique_ptr<Sprite>(sprite);
	unLockReticle_ = std::unique_ptr<Sprite>(unLockSprite);

	cameraToReticle_ = 50;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Reticle::Update(const bool& isLockOn, const WorldTransform& worldTransform, const ViewProjection& viewProjection) {
	if (!isLockOn) {
		Move();
	}

	if (worldTransform.parent_) {
		// 3Dレティクルの位置をScreen上からworld上の位置を求める
		ScreenToWorldOf3DReticle(viewProjection);
	} else {
		// 3Dレティクルのworld座標を計算
		Calculate3DReticleWorldPos(worldTransform);
	}

	// ロックオン状態なら
	if (target_ != nullptr) {
		// 座標の計算を行う
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetWorldPosition();
		// ワールド座標からスクリーン座標に変換
		Vector3 positionScreen = target_->GetScreenPosition(positionWorld, viewProjection);
		// Vector2に格納
		Vector2 spritePos(positionScreen.x, positionScreen.y);
		// 設定
		lockOnReticle_->SetPosition(spritePos);

		worldTransform3D_.translation_ = target_->GetWorldTransform().translation_;
	}

	// unLockOn用の座標の設定しておく
	unLockReticle_->SetPosition(lockOnReticle_->GetPosition());

	if (worldTransform.parent_) {
		matWorld_ = Multiply(worldTransform3D_.matWorld_, worldTransform.parent_->matWorld_);
	}

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

void Reticle::Draw2DReticle(const bool& isLockOnMode) {
	if (isLockOnMode) {
		lockOnReticle_->Draw();
	} else {
		unLockReticle_->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- Reticleを移動させる ------------------- //

void Reticle::Move() {
	// joyState------------------------------------------------------------
	Vector2 spritePos = lockOnReticle_->GetPosition();

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * 8.0f;
		spritePos.y -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 8.0f;

		// 反映
		lockOnReticle_->SetPosition(Vector2(static_cast<float>(spritePos.x), static_cast<float>(spritePos.y)));
	}
}

// ------------------- Z注目を行う ------------------- //

void Reticle::ZTargeting() {

}

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
	Vector2 spritePos = lockOnReticle_->GetPosition();
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

// ------------------- レティクルのスクリーン座標を取得(3D) ------------------- //

bool Reticle::IsOutOfRange(const BaseEnemy* enemy, const ViewProjection& viewProjection) {
	Vector3 positionView = GetViewPosition(enemy, viewProjection);

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(
			std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
			positionView.z);

		// 角度条件チェック
		if (std::abs(arcTangent) <= angleRange_) {
			return false;
		}
	}

	// 範囲外である
	return true;
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
	lockOnReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

// ------------------- レティクルのスクリーン座標を設定(2D) ------------------- //

Vector3 Reticle::GetViewPosition(const BaseEnemy* enemy, const ViewProjection& viewProjection) {
	// 敵のロックオン座標取得
	Vector3 positionWorld = enemy->GetWorldPosition();
	// ワールド→ビュー座標変換
	Vector3 positionView = Transform(positionWorld, viewProjection.matView);

	return positionView;
}
