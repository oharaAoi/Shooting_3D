#include "MidEnemyRootState.h"
#include "GameObject/MidMobEnemy.h"

MidEnemyRootState::MidEnemyRootState(MidMobEnemy* baseEnemy)
	: BaseEnemyState("State_Root", baseEnemy) {
	Init();
}

MidEnemyRootState::~MidEnemyRootState() {}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void MidEnemyRootState::Init() {
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void MidEnemyRootState::Update() {
}
