#include "BossRootState.h"
#include "GameObject/BossEnemy.h"

BossRootState::BossRootState(BossEnemy* baseEnemy) 
	: BaseEnemyState("State_Root", baseEnemy) {
	Init();
}

BossRootState::~BossRootState() {
}

///////////////////////////////////////////////////////////
// 初期化関数
///////////////////////////////////////////////////////////

void BossRootState::Init() {
	work_.moveTime = 0;
	work_.attackTime = 120;
}

///////////////////////////////////////////////////////////
// 更新関数
///////////////////////////////////////////////////////////

void BossRootState::Update() {
	enemy_->Move();
	// フレームをカウントし、一定時間過ぎたら攻撃フェーズに移動する
	work_.moveTime++;
	if (work_.moveTime > work_.attackTime) {
		enemy_->SetBehaviorRequest(EnemyBehavior::kAttack);
		work_.moveTime = 0;
	}
}

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

