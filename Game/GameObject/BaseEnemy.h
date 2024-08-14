#pragma once
#include "GameObject/BaseCharacter.h"

/// <summary>
/// Enemyの基本となるクラス
/// </summary>
class BaseEnemy :
    public BaseCharacter {

public:

    BaseEnemy() = default;
    ~BaseEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(std::vector<Model*> models) override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection) const override;

protected:

	// ------------ キャラクターの情報に関する変数 ------------ // 
	uint32_t hp_ = 0;

};

