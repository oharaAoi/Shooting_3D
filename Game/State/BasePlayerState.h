#pragma once
#include "State/BaseCharacterState.h"

class Player;

class BasePlayerState
    : public BaseCharacterState {
public:

	BasePlayerState(const std::string& name, Player* player) : name_(name), player_(player) {};
	virtual	~BasePlayerState() override = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual	void Init() override = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual	void Update() override = 0;

protected:
	// 状態名
	std::string name_;
	// 状態の対象
	Player* player_ = nullptr;
};

