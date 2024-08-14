#pragma once
#include <cassert>
#include <map>
#include <string>
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Math/MyMath.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

static const int32_t kLifeTime = 60 * 5;

class BaseBullet {
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
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// 生存しているかを判断するフラグ
	/// </summary>
	/// <returns></returns>
	const bool GetIsDead() const { return isDead_; }

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

