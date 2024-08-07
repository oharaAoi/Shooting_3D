#include "Player.h"

Player::Player(std::vector<Model*> models) { Init(models); }
Player::~Player() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init(std::vector<Model*> models) {
	BaseCharacter::Init(models);

	// partsの親の設定
	worldTransforms_[PlayerParts::Parts_Body].parent_ = &worldTransform_;
	worldTransforms_[PlayerParts::Parts_Face].parent_ = &worldTransforms_[PlayerParts::Parts_Body];
	worldTransforms_[PlayerParts::Parts_LeftArm].parent_ = &worldTransforms_[PlayerParts::Parts_Body];
	worldTransforms_[PlayerParts::Parts_RightArm].parent_ = &worldTransforms_[PlayerParts::Parts_Body];

	// ---------------------------------------------
	// ↓ 調整項目
	// ---------------------------------------------
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	adjustItem->CreateGroup(groupName);
	// 項目の追加
	adjustItem->AddItem(groupName, "face_Translation", worldTransforms_[PlayerParts::Parts_Face].translation_);
	adjustItem->AddItem(groupName, "L_Arm_Translation", worldTransforms_[PlayerParts::Parts_LeftArm].translation_);
	adjustItem->AddItem(groupName, "R_Arm_Translation", worldTransforms_[PlayerParts::Parts_RightArm].translation_);

	ApplyAdjustItems();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	//Move();

	// 計算転送
	BaseCharacter::Update();
	// ImGuiの編集
	EditImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw(const ViewProjection& viewProjection) {
	BaseCharacter::Draw(viewProjection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Playerの行動関連
//////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Move() {
	XINPUT_STATE joyState;
	bool isMoving = false;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		const float speed = 0.3f;
		float targetAngle = 0;

		velocity_ = {
			static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX,
			0.0f,
			static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX,
		};

		// スティックの押し込みが閾値を超えていたら移動可能にする
		if (Length(velocity_) > threshold) {
			isMoving = true;
		}

		velocity_ = Normalize(velocity_) * speed;

		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 cameraRotate = MakeRotateXYZMatrix(viewProjection_->rotation_);
		velocity_ = TransformNormal(velocity_, cameraRotate);

		// 移動処理
		if (isMoving) {
			worldTransform_.translation_ += velocity_;
			// 方向を取得しておく
			direction_ = velocity_;
			// 目標角度の算出
			targetAngle = std::atan2f(velocity_.x, velocity_.z);
		} 

		// --------------------------------------
		// プレイヤーの向きを移動方向に合わせる
		// --------------------------------------
		// 回転させる
		// y軸まわり(z軸方向とy軸方向のベクトルで求まる)
		if (isMoving) {
			worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle, 0.1f);
		} else {
			//worldTransform_.rotation_.y = std::atan2f(direction_.x, direction_.z);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目関連
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- ImGuiの編集 ------------------- //

void Player::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("head", &worldTransforms_[PlayerParts::Parts_Face].translation_.x, 0.1f);
	ImGui::DragFloat3("leftArm", &worldTransforms_[PlayerParts::Parts_LeftArm].translation_.x, 0.1f);
	ImGui::DragFloat3("rightArm", &worldTransforms_[PlayerParts::Parts_RightArm].translation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

// ------------------- 調整項目の適応 ------------------- //
void Player::ApplyAdjustItems() {
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	const char* groupName = "Player";

	worldTransforms_[PlayerParts::Parts_Face].translation_ = adjustItem->GetValue<Vector3>(groupName, "face_Translation");
	worldTransforms_[PlayerParts::Parts_LeftArm].translation_ = adjustItem->GetValue<Vector3>(groupName, "L_Arm_Translation");
	worldTransforms_[PlayerParts::Parts_RightArm].translation_ = adjustItem->GetValue<Vector3>(groupName, "R_Arm_Translation");
}
