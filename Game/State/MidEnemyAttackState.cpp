#include "MidEnemyAttackState.h"
#include "GameObject/MidMobEnemy.h"

MidEnemyAttackState::MidEnemyAttackState(MidMobEnemy* baseEnemy) 
	: BaseEnemyState("State_Attack", baseEnemy) {
	Init();
}

MidEnemyAttackState::~MidEnemyAttackState() {
}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void MidEnemyAttackState::Init() {
	work_.attackTime = 240;
	work_.attackCoolTime = 80;
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void MidEnemyAttackState::Update() {
	enemy_->Move();

	if (--work_.attackCoolTime <= 0) {
		enemy_->Attack();
		work_.attackCoolTime = 80;
	}

	if (--work_.attackTime <= 0) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kRoot);
		work_.attackTime = 240;
	}
}