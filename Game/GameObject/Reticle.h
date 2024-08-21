#pragma once
// STL
#include <cassert>
#include <map>
#include <string>
// KamataEngine
#include "WinApp.h"
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Sprite.h"
#include "TextureManager.h"
// Math
#include "Math/MyMath.h"
// Loader
#include "Loader/ModelLoader.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

/// <summary>
/// Playerのレティクル
/// </summary>
class Reticle {
public:

	Reticle();
	~Reticle();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 2Dのレティクルを描画する
	/// </summary>
	void Draw2DReticle(const bool& isLockOnMode);

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 3Dレティクルのworld座標を計算
	/// </summary>
	void Calculate3DReticleWorldPos(const WorldTransform& worldTransform);

	/// <summary>
	/// スクリーン座標からワールド座標を取得して配置する関数(コントローラー)
	/// </summary>
	void ScreenToWorldOf3DReticle(const ViewProjection& viewProjection);

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Parentの設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// レティクルのスクリーン座標を取得(3D)
	/// </summary>
	/// <returns></returns>
	Vector3 Get3DReticleWorldPos();

	/// <summary>
	/// レティクルのスクリーン座標を取得(2D)
	/// </summary>
	/// <returns></returns>
	Vector2 Get2DReticleScreenPos() { return lockOnReticle_->GetPosition(); };

	/// <summary>
	/// レティクルのスクリーン座標を設定
	/// </summary>
	/// <returns></returns>
	void Set2DReticleScreenPos(const Vector3& pos) { lockOnReticle_->SetPosition({ pos.x, pos.y }); }

	/// <summary>
	/// 3Dのレティクルのワールドから2Dへ
	/// </summary>
	void Set3DReticleTo2DReticle(const ViewProjection& viewProjection);

	/// <summary>
	/// レティクルのある位置を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetReticlePosition();

	const WorldTransform& Get3DReticleTransform() { return worldTransform3D_; }

	const Matrix4x4 GetMatWorld() const { return matWorld_; }

private:

	Model* model_ = nullptr;
	// 3dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3D_;
	// 2dReticle
	std::unique_ptr<Sprite> lockOnReticle_;
	std::unique_ptr<Sprite> unLockReticle_;
	// 2dReticleResource
	uint32_t lockOnReticleHandle_;
	uint32_t unLockReticleHandle_;

	float cameraToReticle_;

	Matrix4x4 matWorld_;

	// 2Dレティクルを使用するか3Dレティクルを使用するか
	bool is3dReticle_ = false;
};

