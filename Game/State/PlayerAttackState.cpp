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
	workAttack_.coolTime_ = 0;
	workAttack_.exitStateTime_ = 40;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerAttackState::Update() {
	player_->Move();

	// 弾を撃つクールタイム
	if (workAttack_.coolTime_ > 0) {
		workAttack_.coolTime_--;
		return;
	}

	// 弾を撃つ
	Shot();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃を行う関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Shot() {
	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }
	
	if (joyState.Gamepad.bRightTrigger > 50) {
		const float kBulletSpeed = 0.8f;
		// parentが登録されていたら
		Vector3 velocity;
		Reticle* reticle = player_->GetReticle();
		if (player_->GetReticle()->GetIsLockOn()) {
			velocity = reticle->GetTargetTransform() - player_->GetWorldTransform().translation_;

		} else {
			if (player_->GetWorldTransform().parent_) {
				Matrix4x4 reticleMat = reticle->Get3DReticleTransform().matWorld_ * Inverse(player_->GetWorldTransform().parent_->matWorld_);
				velocity = Transform({ 0,0,0 }, reticleMat) - player_->GetTranslation();
			} else {
				velocity = player_->GetReticle()->Get3DReticleWorldPos() - player_->GetWorldPosition();
			}
		}

		// 自機から昇順オブジェクトへのベクトル
		velocity = Normalize(velocity) * kBulletSpeed;

		//if (player_->GetIsBossBattle()) {
		//	// 速度ベクトルを自機の向きに合わせて回転させる
		//	velocity = TransformNormal(velocity, player_->GetMatWorld());
		//}

		// bulletのインスタンスを生成する
		player_->AddBulletList(velocity);
		// coolTimeを設定する
		workAttack_.coolTime_ = 15;
	}
}
