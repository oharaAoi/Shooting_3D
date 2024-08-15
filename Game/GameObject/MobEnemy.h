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
	void Move();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;


///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const override;

private:

	

};

