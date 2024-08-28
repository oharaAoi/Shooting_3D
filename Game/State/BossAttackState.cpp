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
	work_.attackTime = 240;
	work_.attackCoolTime = 60;
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void BossAttackState::Update() {
	enemy_->Move();
	if (--work_.attackCoolTime <= 0) {
		enemy_->Attack();
		work_.attackCoolTime = 60;
	}
	
	if (--work_.attackTime <= 0) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kRoot);
		work_.attackTime = 240;
	}
}
