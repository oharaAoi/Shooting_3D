#pragma once
#include "State/BasePlayerState.h"
#include "Manager/AudioManager.h"

class Player;

/// <summary>
/// Playerの通常の振る舞い
/// </summary>
class PlayerRootState :
	public BasePlayerState {
public:

	PlayerRootState(Player* player);
	~PlayerRootState();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 走る
	/// </summary>
	void Dash();

	/// <summary>
	/// 攻撃を行う
	/// </summary>
	void Attack();

private:

	uint32_t dashCoolTime_ = 0;

};

