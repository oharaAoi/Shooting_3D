#include "FollowCamera.h"

FollowCamera::FollowCamera() { Init(); }
FollowCamera::~FollowCamera() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::Init() {
	viewProjection_.Initialize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::Update() {
	Rotate();

	Vector3 offset = CalculationOffset();
	viewProjection_.translation_ = interTarget_ + offset;

	if (target_) {
		interTarget_ = Lerp(interTarget_, target_->translation_, 0.3f);
	}

	viewProjection_.UpdateMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　カメラに関する移動系
//////////////////////////////////////////////////////////////////////////////////////////////////
void FollowCamera::Rotate() {
	XINPUT_STATE joyState;

	//if (lockOn_ != nullptr) {
	//	// カメラを敵の方向に向ける
	//	// カメラと敵のベクトルをとる
	//	Vector3 diff = lockOn_->GetCenterPos() - target_->translation_;
	//	diff = Normalize(diff);

	//	viewProjection_.rotation_.y = std::atan2f(diff.x, diff.z);
	//	/*float xzLength = Length({ diff.x, 0, diff.z });
	//	viewProjection_.rotation_.x = std::atan2f(-diff.y, xzLength);*/

	//} else

	 if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float speed = 0.1f;
		destinationAngleY_ += (float)static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * speed;
		// 
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			destinationAngleY_ = 0;
		}

		viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
	}
}

// ------------------- ImGuiの編集 ------------------- //

void FollowCamera::SetTarget(const WorldTransform* target) {
	target_ = target;
	ResetTarget();
}

// ------------------- ImGuiの編集 ------------------- //

void FollowCamera::ResetTarget() {
	// 対象がいたら
	if (target_) {
		// 追従対象者の座標・角度の記憶
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = viewProjection_.rotation_.y;

	// 追従対象からのオフセット
	Vector3 offset = CalculationOffset();
	viewProjection_.translation_ = interTarget_ + offset;
}

// ------------------- ImGuiの編集 ------------------- //

Vector3 FollowCamera::CalculationOffset() {
	Vector3 offset = { 0.0f, 0.0f, -30.0f };
	// 回転行列の合成
	Matrix4x4 matRotate = MakeRotateXYZMatrix(viewProjection_.rotation_);
	offset = TransformNormal(offset, matRotate);

	return offset;
}
