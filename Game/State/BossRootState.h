#pragma once
#include "State/BaseEnemyState.h"

class BossEnemy;

class BossRootState :
    public BaseEnemyState {

public:

    BossRootState(BossEnemy* baseEnemy);
    ~BossRootState();

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

