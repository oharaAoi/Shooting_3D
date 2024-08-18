#include "PlayerRootState.h"
#include "GameObject/Player.h"
#include "State/BaseCharacterState.h"

PlayerRootState::PlayerRootState(Player* player) 
	: BasePlayerState("State_Root", player) {
	Init();
}

PlayerRootState::~PlayerRootState() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerRootState::Init() {
	dashCoolTime_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerRootState::Update() {
	player_->Move();
	Dash();
	Attack();
}

// ------------------- Playerを走らせる状態に変更させる ------------------- //

void PlayerRootState::Dash() {
	// クールタイムならダッシュ出来なようにする
	if (dashCoolTime_ != 0) {
		dashCoolTime_--;
		return;
	}

	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		// 状態の変更
		player_->SetBehaviorRequest(PlayerBehavior::kDash);
		// 角度から方向にする
		Vector3 velocity = TransformNormal({ 0,0,0.7f }, player_->GetWorldTransform().matWorld_);
		dashCoolTime_ = 40;
	}
}

// ------------------- 攻撃を行う状態に変更させる ------------------- //

void PlayerRootState::Attack() {
	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		player_->SetBehaviorRequest(PlayerBehavior::kAttack);
	}
}
