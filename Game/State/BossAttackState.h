#pragma once
#include "State/BaseEnemyState.h"

class BossEnemy;

class BossAttackState :
    public BaseEnemyState {

public:

    BossAttackState(BossEnemy* baseEnemy);
    ~BossAttackState();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

private:

};

