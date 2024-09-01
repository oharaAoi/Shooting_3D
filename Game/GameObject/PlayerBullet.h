#pragma once
#include "GameObject/BaseBullet.h"
#include "Manager/AudioManager.h"

class PlayerBullet :
	public BaseBullet {
public:

	PlayerBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent, const uint32_t& collisionCount);
	~PlayerBullet();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pos"></param>
	/// <param name="velocity"></param>
	/// <param name="rotation"></param>
	void Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) const override;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const override;

	/// <summary>
	/// 当たった回数を決定する
	/// </summary>
	/// <param name="collisionCount"></param>
	void SetCollisionCount(const uint32_t& collisionCount) { collisionCount_ = collisionCount; }

private:

};

