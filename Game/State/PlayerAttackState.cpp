#include "PlayerAttackState.h"
#include "GameObject/Player.h"

PlayerAttackState::PlayerAttackState(Player* player)
	: BasePlayerState("State_Root", player) {
	Init();
}

PlayerAttackState::~PlayerAttackState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerAttackState::Init() {
	workAttack_.coolTime_ = 15;
	workAttack_.exitStateTime_ = 40;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerAttackState::Update() {
	player_->Move();

	Shot();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃を行う関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Shot() {
	/*XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }*/

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 0.6f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 自機から昇順オブジェクトへのベクトル
		velocity = Normalize(velocity) * kBulletSpeed;
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, player_->GetMatWorld());
		// bulletのインスタンスを生成する
		player_->AddBulletList(velocity);
	}
}
