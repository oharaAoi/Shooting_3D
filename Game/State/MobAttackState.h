#pragma once
#include "State/BaseEnemyState.h"

class MobEnemy;

class MobAttackState :
    public BaseEnemyState {

public:

	struct Work {
		uint32_t attackTime;
	};

public:

    MobAttackState(MobEnemy* baseEnemy);
    ~MobAttackState();

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

	/// <summary>
	/// 攻撃を行う関数
	/// </summary>
	void Attack();

private:

	/// <summary>
	/// 攻撃のクールタイム
	/// </summary>
	uint32_t attackCoolTime_ = 0;

	Work work_;

};

