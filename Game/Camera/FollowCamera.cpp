#include "FollowCamera.h"

FollowCamera::FollowCamera() { Init(); }
FollowCamera::~FollowCamera() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::Init() {
	viewProjection_.Initialize();
	worldTransform_.Initialize();
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
	// worldTransformを設定する
	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("view.rotate", &viewProjection_.rotation_.x, 0.01f);
	ImGui::DragFloat("destinationAngleY", &destinationAngleY_, 0.01f);
	ImGui::DragFloat("destinationAngleX", &destinationAngleX_, 0.01f);
	ImGui::End();
#endif // _DEBUG

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　カメラに関する移動系
//////////////////////////////////////////////////////////////////////////////////////////////////
void FollowCamera::Rotate() {
	XINPUT_STATE joyState;

	if ((reticle_ != nullptr) && (target_ != nullptr)) {
		Vector3 diff = reticle_->GetTargetWorldPos() - target_->translation_;
		diff = Normalize(diff);
		float xzLenght = Length({ diff.x, 0,  diff.z });
		float targetAngleX = std::atan2f(-diff.y, xzLenght);
		viewProjection_.rotation_.y = std::atan2f(diff.x, diff.z);
		viewProjection_.rotation_.x = targetAngleX;
	
	} else if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float speed = 0.05f;
		destinationAngleY_ += (float)static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * speed;
		destinationAngleX_ += (float)static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * speed;

		if (destinationAngleX_ > 1.0f) {
			destinationAngleX_ = 1.0f;
		}
		if (destinationAngleX_ < -1.0f) {
			destinationAngleX_ = -1.0f;
		}

		//// 
		//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
		//	destinationAngleY_ = 0;
		//}

		viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
		viewProjection_.rotation_.x = LerpShortAngle(viewProjection_.rotation_.x, destinationAngleX_, 0.1f);
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
	Vector3 offset = { 0.0f, 2.0f, -20.0f };
	// 回転行列の合成
	Matrix4x4 matRotate = MakeRotateXYZMatrix(viewProjection_.rotation_);
	offset = TransformNormal(offset, matRotate);

	return offset;
}

// ------------------- ImGuiの編集 ------------------- //

void FollowCamera::LockOnMove(const Vector3& rotation) {
	Vector3 sub = rotation - viewProjection_.translation_;
	viewProjection_.rotation_.y = std::atan2f(sub.y, sub.z);
}
