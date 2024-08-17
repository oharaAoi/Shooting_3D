#pragma once
// STL
#include <cassert>
#include <map>
#include <string>
// KamataEngine
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
// Math
#include "Vector3.h"
#include "Math/MyMath.h"
// Collision
#include "Collision/Collider.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

static const int32_t kLifeTime = 60 * 5;

class BaseBullet
	: public Collider {
public:

	BaseBullet() = default;
	~BaseBullet() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(Model* model, const Vector3& pos, const Vector3& velocity, const Vector3& rotation);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection) const;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other) override {};

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() const override { return Vector3(); };

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// 生存しているかを判断するフラグ
	/// </summary>
	/// <returns></returns>
	const bool GetIsDead() const { return isDead_; }

	// ------------ Transform ------------ // 
	void SetParent(const WorldTransform* parent);

protected:

	// 3Dモデルのポインタ
	Model* model_;

	// worldTrasform
	WorldTransform worldTransform_;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	// ------------ 移動に関する変数 ------------ // 
	// 方向
	Vector3 direction_;
	// 速度
	Vector3 velocity_;

};

