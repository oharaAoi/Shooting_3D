#pragma once
#include "State/BaseCharacterState.h"

class BaseEnemy;

class BaseEnemyState :
    public BaseCharacterState {

public:

	BaseEnemyState(const std::string& name, BaseEnemy* baseEnemy) : name_(name), enemy_(baseEnemy) {};
	virtual	~BaseEnemyState() override = default;

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
	BaseEnemy* enemy_ = nullptr;

};

