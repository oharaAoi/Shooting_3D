#pragma once
#include "State/BaseEnemyState.h"

class MobEnemy;

class MobRootState :
    public BaseEnemyState {
public:

	struct WorkRoot {
		uint32_t moveTime;	// 動いている時間
		uint32_t attackTime;// 攻撃するまでの時間
	};

	struct Floating {
		float parameter;
		float amplitude;
		uint32_t period;
	};

public:

    MobRootState(MobEnemy* baseEnemy);
    ~MobRootState();

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

	Floating floating_;
};

