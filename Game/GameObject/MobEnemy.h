#pragma once
#include "GameObject/BaseEnemy.h"

enum EnmeyParts {
	Enmey_Body = 0,
};

class MobEnemy :
    public BaseEnemy {
public:

    MobEnemy(std::vector<Model*> models);
    ~MobEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::vector<Model*> models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) const override;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// Playerを動かす
	/// </summary>
	void Move() override;

private:

	

};

