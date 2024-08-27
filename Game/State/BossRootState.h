#pragma once
#include "State/BaseEnemyState.h"

class BossEnemy;

class BossRootState :
    public BaseEnemyState {
public:

	struct WorkRoot {
		uint32_t moveTime;	// 動いている時間
		uint32_t attackTime;// 攻撃するまでの時間
	};

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

	WorkRoot work_;

};

