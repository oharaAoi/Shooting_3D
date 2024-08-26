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
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void MidEnemyAttackState::Update() {
	enemy_->Move();
}