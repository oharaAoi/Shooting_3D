#include "RailCamera.h"

RailCamera::RailCamera(const Vector3& translation, const Vector3& rotation) {
	Init(translation, rotation);
}
RailCamera::~RailCamera() {
}

void RailCamera::Init(const Vector3& translation, const Vector3& rotation) {
	viewProjection_.Initialize();
	// worldTrasnformの初期設定
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = translation;

	localTranslation_ = { 0.0f, 0.0f, 0.0f };
	localRotate_ = { 0.0f, 0.0f, 0.0f };
}

void RailCamera::Update() {
	// --------------------------------------
	// カメラを一定フレームで移動させる
	// --------------------------------------
	frameCount_++;
	if (frameCount_ > 5) {
		RailMove();
		frameCount_ = 0;
	}

	// --------------------------------------
	// 行列の計算を行う
	// --------------------------------------
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェトのワールド行列からビューを計算
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	// 計算転送
	worldTransform_.UpdateMatrix();
}

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

// ------------------- レールを移動させる処理 ------------------- //

void RailCamera::RailMove() {
	eyeIndex_++;
	if (eyeIndex_ >= static_cast<uint32_t>(controlPoints_.size()) - 1) {
		eyeIndex_ = 0;
	}

	// ここを編集する必要がある
	uint32_t forwardIndex = eyeIndex_ + 5;
	if (controlPoints_.size() <= forwardIndex) {
		forwardIndex = static_cast<uint32_t>(controlPoints_.size()) - 1;
	}

	// eyeの座標をワールドトランスフォームに代入する
	worldTransform_.translation_ = controlPoints_[eyeIndex_];
	// forwardの座標からカメラの回転角をもとめる
	Vector3 forwardPos = controlPoints_[forwardIndex];

	Vector3 diff = Normalize(forwardPos - worldTransform_.translation_);
	worldTransform_.rotation_.y = localRotate_.y + std::atan2f(diff.x, diff.z);
	float xzLenght = Length({ diff.x, 0, diff.z });
	worldTransform_.rotation_.x = localRotate_.x + std::atan2f(-diff.y, xzLenght);
}

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

// ------------------- worldTransform ------------------- //

const Vector3 RailCamera::GetWorldPosition() const {
	Vector3 result{};
	// 行列から平行移動分を取得
	result = Transform({ 0, 0, 0 }, worldTransform_.matWorld_);

	return result;
}
