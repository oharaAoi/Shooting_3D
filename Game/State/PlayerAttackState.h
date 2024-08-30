#pragma once
#include "State/BasePlayerState.h"

class Player;

class PlayerAttackState :
	public BasePlayerState {
public:

	struct WorkAttack {
		uint32_t coolTime_ = 0;		// 攻撃のcoolTimeを表す変数
		uint32_t exitStateTime_ = 0;		// 攻撃状態を解除するまでの時間
	};
public:

	PlayerAttackState(Player* player);
	~PlayerAttackState();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 弾を撃つ
	/// </summary>
	void Shot();

	/// <summary>
	/// 走りに入る処理
	/// </summary>
	void Dash();

private:

	WorkAttack workAttack_;

};

