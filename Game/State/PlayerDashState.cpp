#include "PlayerDashState.h"
#include "GameObject/Player.h"

PlayerDashState::PlayerDashState(Player* player)
	: BasePlayerState("State_Dash", player) {
	Init(); 
}

PlayerDashState::~PlayerDashState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerDashState::Init() {
	workDash_.dashParameter_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerDashState::Update() {
	Vector3 velocity(0, 0, 1.0f);
	velocity = TransformNormal(velocity, player_->GetWorldTransform().matWorld_);

	Vector3 translate = player_->GetTranslation();
	translate += velocity;

	// ダッシュ時間
	const uint32_t behaviorDashTime = 20;

	// 既定時間で通常に戻る
	if (++workDash_.dashParameter_ > behaviorDashTime) {
		player_->SetBehaviorRequest(PlayerBehavior::kRoot);
	}

	// Playerにセットする
	player_->SetTranslation(translate);
}
