#pragma once
#include "GameObject/BaseBullet.h"

class BossBullet :
    public BaseBullet {
public:

    BossBullet(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation, const WorldTransform* parent);
    ~BossBullet();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model">: modelのポインタ</param>
	/// <param name="pos">: 座標</param>
	/// <param name="velocity">: 速度</param>
	/// <param name="rotation">: 回転</param>
	/// <param name="parent">: 親のworldTransformのポインタ</param>
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

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////


private:

	const float kBulletSpeed_ = 0.6f;

};

