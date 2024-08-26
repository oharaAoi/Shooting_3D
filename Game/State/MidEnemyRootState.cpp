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
	work_.moveTime = 0;
	work_.attackTime = 120;
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void MidEnemyRootState::Update() {
	enemy_->Move();
	// フレームをカウントし、一定時間過ぎたら攻撃フェーズに移動する
	/*work_.moveTime++;
	if (work_.moveTime > work_.attackTime) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kAttack);
		work_.moveTime = 0;
	}*/
}

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

void MidEnemyRootState::AnimationGimmick() {
	
}
