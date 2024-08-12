#pragma once
#include "State/BasePlayerState.h"

class Player;

class PlayerDashState :
    public BasePlayerState {
public:

	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};

public:

	PlayerDashState(Player* player);
	~PlayerDashState();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

	WorkDash workDash_;

};

