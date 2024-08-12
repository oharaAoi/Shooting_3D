#pragma once
#include <String>
#include "Math/MyMath.h"

class BaseCharacter;

/// <summary>
/// キャラクターの状態を表したクラス
/// </summary>
class BaseCharacterState {
public:

	BaseCharacterState() {};
	virtual	~BaseCharacterState() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

protected:

	// 状態名
	std::string name_;
	// 状態の対象
	BaseCharacter* baseCharacter_ = nullptr;

};

