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
	void Update(const std::list<std::unique_ptr<BaseEnemy>>& enemyList,
				const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 2Dのレティクルを描画する
	/// </summary>
	void Draw2DReticle();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ↓　メンバ関数
	//////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Reticleの移動
	/// </summary>
	void Move();

	/// <summary>
	/// LockOnを行う関数
	/// </summary>
	/// <param name="enemyList"></param>
	/// <param name="viewProjection"></param>
	void LockOn(const std::list<std::unique_ptr<BaseEnemy>>& enemyList, const ViewProjection& viewProjection);

	/// <summary>
	/// LockOn対象を変更する
	/// </summary>
	/// <param name="viewProjection"></param>
	void ChangetLockOnTarget(const ViewProjection& viewProjection);

	/// <summary>
	/// targetが生きているか
	/// </summary>
	void CheckTargerAlive();

	/// <summary>
	/// 3Dレティクルのworld座標を計算
	/// </summary>
	void Calculate3DReticleWorldPos(const WorldTransform& worldTransform);

	/// <summary>
	/// スクリーン座標からワールド座標を取得して配置する関数(コントローラー)
	/// </summary>
	void ScreenToWorldOf3DReticle(const ViewProjection& viewProjection);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void Search(const std::list<std::unique_ptr<BaseEnemy>>& enemies, const ViewProjection& viewProjection);

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

	// ------------ parent ------------ //
	void SetParent(const WorldTransform* parent);

	// ------------ 3dReticleの位置 ------------ //
	Vector3 Get3DReticleWorldPos();

	// ------------ 2dReticleの位置 ------------ //
	Vector2 Get2DReticleScreenPos() { return lockOnReticle_->GetPosition(); };
	void SetLockOnScreenPos(const Vector3& pos) { lockOnReticle_->SetPosition({ pos.x, pos.y }); }

	// ------------ 3dReticleから3dReticleを設定する ------------ //
	void Set3DReticleTo2DReticle(const ViewProjection& viewProjection);

	// ------------ worldTransoform ------------ //
	const WorldTransform& Get3DReticleTransform() { return worldTransform3D_; }
	const Matrix4x4 GetMatWorld() const { return matWorld_; }

	// ------------ bool ------------ //
	const bool GetIsLockOn() const { return isLockOn_; }

	// ------------ Target関連 ------------ //
	Vector3 GetTargetWorldPos() const { return target_->GetWorldPosition(); }
	Vector3 GetTargetTransform() const { return target_->GetWorldTransform().translation_; }
	void SetTarget(BaseEnemy* enemy) { target_ = enemy; }
	bool GetIsTargetAlive() {
		if (target_ == nullptr) {
			return false;
		}
		return true;
	};

private:

	Model* model_ = nullptr;
	// 3dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3D_;
	// 2dReticle
	std::unique_ptr<Sprite> lockOnReticle_;
	std::list<std::unique_ptr<Sprite>> canLockReticle_;
	// 2dReticleResource
	uint32_t lockOnReticleHandle_;
	uint32_t canLockReticleHandle_;

	float cameraToReticle_;

	Matrix4x4 matWorld_;

	Vector3 playerTranslation_;

	// ------------ z注目用の変数 ------------ //
	// ロックオン対象
	const BaseEnemy* target_ = nullptr;
	// ロックオン可能な敵のリスト
	std::list<const BaseEnemy*> canLockOnList_;
	bool isLockOn_;

	// parameter
	// 最小距離
	float minDistance_ = 5.0f;
	// 最大距離
	float maxDistance_ = 50.0f;
	// 角度範囲
	float angleRange_ = 10.0f * kDegreeToRadian;
};

