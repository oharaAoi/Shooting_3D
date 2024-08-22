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
// GameObject
#include "GameObject/BaseEnemy.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

const static float kDegreeToRadian = std::numbers::pi_v<float> / 6.0f;

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
	void Update(const bool& isLockOn, const WorldTransform& worldTransform, const ViewProjection& viewProjection);

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
	/// Reticleの移動
	/// </summary>
	void Move();

	void ZTargeting();

	/// <summary>
	/// 3Dレティクルのworld座標を計算
	/// </summary>
	void Calculate3DReticleWorldPos(const WorldTransform& worldTransform);

	/// <summary>
	/// スクリーン座標からワールド座標を取得して配置する関数(コントローラー)
	/// </summary>
	void ScreenToWorldOf3DReticle(const ViewProjection& viewProjection);

	// ------------ z注目用の関数 ------------ //

	/// <summary>
	/// 
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="viewProjection"></param>
	/// <returns></returns>
	bool IsOutOfRange(const BaseEnemy* enemy, const ViewProjection& viewProjection);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="viewProjection"></param>
	/// <returns></returns>
	Vector3 GetViewPosition(const BaseEnemy* enemy, const ViewProjection& viewProjection);

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
	void SetLockOnScreenPos(const Vector3& pos) { lockOnReticle_->SetPosition({ pos.x, pos.y }); }

	/// <summary>
	/// 3Dのレティクルのワールドから2Dへ
	/// </summary>
	void Set3DReticleTo2DReticle(const ViewProjection& viewProjection);

	const WorldTransform& Get3DReticleTransform() { return worldTransform3D_; }

	const Matrix4x4 GetMatWorld() const { return matWorld_; }

	/// <summary>
	/// Targetを設定する
	/// </summary>
	/// <param name="enemy"></param>
	void SetTarget(BaseEnemy* enemy) { target_ = enemy; }

	/// <summary>
	/// 敵の中心座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetWorldPos() const { return target_->GetWorldPosition(); }

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

	// ------------ z注目用の変数 ------------ //
	// ロックオン対象
	const BaseEnemy* target_ = nullptr;

	// parameter
	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 30.0f;
	// 角度範囲
	float angleRange_ = 20.0f * kDegreeToRadian;
};

