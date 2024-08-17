#pragma once
#include "GameObject/BaseCharacter.h"

enum EnemyType {
	Type_Mob = 0,
	Type_MidBoss,
	Type_Boss
};

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

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other) override {};

	/// <summary>
	/// ImGui
	/// </summary>
	virtual void EditImGui() {};

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() const override;

	// ------------ worldTransform ------------ // 
	void SetParent(const WorldTransform* parent);


protected:

	// ------------ キャラクターの情報に関する変数 ------------ // 
	uint32_t hp_ = 0;

};

