#include "BossAttackState.h"
#include "GameObject/BossEnemy.h"

BossAttackState::BossAttackState(BossEnemy* baseEnemy) 
	: BaseEnemyState("State_Attack", baseEnemy) {
	Init();
}

BossAttackState::~BossAttackState() {
}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void BossAttackState::Init() {
	work_.attackTime = 180;
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void BossAttackState::Update() {
	enemy_->Move();
	if (--work_.attackTime <= 0) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kRoot);
	}
}
