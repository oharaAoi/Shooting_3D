#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
// Math
#include "Math/MyMath.h"
// GameObject
#include "GameObject/Reticle.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class FollowCamera {
public:

	FollowCamera();
	~FollowCamera();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// カメラを移動させる
	/// </summary>
	void RailMove();

	/// <summary>
	/// カメラを回転させる
	/// </summary>
	void Rotate();

	/// <summary>
	/// 追従対象のリセット
	/// </summary>
	void ResetTarget();

	/// <summary>
	/// カメラのオフセット位置を計算する
	/// </summary>
	/// <returns></returns>
	Vector3 CalculationOffset();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// ViewProjectionを追加する
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// 追従する対象を設定する
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="reticle"></param>
	void SetReticle(const Reticle* reticle) { reticle_ = reticle; }

	/// <summary>
	/// カメラの向きを取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetCameraRotation() const { return viewProjection_.rotation_; }

	void SetCameraRotation(const Vector3& rotation) { viewProjection_.rotation_ = rotation; }

private:

	// ------------ ポインタ ------------ // 
	ViewProjection viewProjection_;

	const WorldTransform* target_ = nullptr;

	const Reticle* reticle_ = nullptr;

	WorldTransform worldTransform_;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};
	float destinationAngleY_ = 0;
	float destinationAngleX_ = 0;
};

