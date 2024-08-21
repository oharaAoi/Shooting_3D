#pragma once
#include "State/BaseEnemyState.h"

class MobEnemy;

class MobRootState :
    public BaseEnemyState {
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


};

