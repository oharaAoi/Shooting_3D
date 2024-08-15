#pragma once
// STL
#include <vector>
// KamataEngine
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include "Input.h"
// Math
#include "Math/MyMath.h"

class RailCamera {
public:

	RailCamera(const Vector3& translation, const Vector3& rotation);
	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(const Vector3& translation, const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// レールを移動させる処理
	/// </summary>
	void RailMove();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	// ------------ viewProjection ------------ //
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	// ------------ worldTransform ------------ //
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// ------------ worldPosition ------------ //
	const Vector3 GetWorldPosition() const;
	void SetWorldTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }

	// ------------ contorlPoint ------------ //
	void SetControlPoints(const std::vector<Vector3> vector) { controlPoints_ = vector; }

	// ------------ worldMatrix ------------ //
	const Matrix4x4 GetWorldMatWorld() const { return worldTransform_.matWorld_; }

private:

	// ------------ worldTransform ------------ // 
	WorldTransform worldTransform_;
	// ------------ viewProjection ------------ // 
	ViewProjection viewProjection_;

	// ------------ カメラの移動に関する変数 ------------ // 
	std::vector<Vector3> controlPoints_;
	uint32_t eyeIndex_ = 0;
	uint32_t frameCount_ = 0;

	Vector3 localTranslation_;	// カメラ自体がどのくらい動いているか
	Vector3 localRotate_;		// カメラ自体の回転
};

