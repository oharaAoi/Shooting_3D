#pragma once
#include "State/BaseEnemyState.h"

class MidMobEnemy;

class MidEnemyAttackState :
    public BaseEnemyState {
public:

    MidEnemyAttackState(MidMobEnemy* baseEnemy);
    ~MidEnemyAttackState();

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

