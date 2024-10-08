#include "MobAttackState.h"
#include "GameObject/MobEnemy.h"

MobAttackState::MobAttackState(MobEnemy* baseEnemy)
	: BaseEnemyState("State_Root", baseEnemy) {
	Init();
}

MobAttackState::~MobAttackState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobAttackState::Init() {
	work_.attackTime = 120;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MobAttackState::Update() {
	enemy_->Move();

	if (!enemy_->GetIsDiscovery()) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kRoot);
	}

	if (attackCoolTime_ > 0) {
		attackCoolTime_--;
		return;
	}
	//Attack();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 衝突時に行う処理 ------------------- //

void MobAttackState::Attack() {
	enemy_->Attack();
	attackCoolTime_ = 180;
}
