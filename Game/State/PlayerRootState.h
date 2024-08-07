#pragma once
#include "State/BaseCharacterState.h"

class Player;

/// <summary>
/// Playerの通常の振る舞い
/// </summary>
class PlayerRootState :
	public BaseCharacterState {
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

private:


};

