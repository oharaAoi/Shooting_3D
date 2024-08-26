#pragma once
#include "State/BaseEnemyState.h"

class MidMobEnemy;

class MidEnemyRootState :
    public BaseEnemyState {

public:

	struct WorkRoot {
		uint32_t moveTime;	// 動いている時間
		uint32_t attackTime;// 攻撃するまでの時間
	};

public:

    MidEnemyRootState(MidMobEnemy* baseEnemy);
    ~MidEnemyRootState();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	void AnimationGimmick();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

private:

	WorkRoot work_;
};

