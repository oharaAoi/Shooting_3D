#pragma once
#include <optional>
#include "Enviroment.h"
// GameObject
#include "GameObject/BaseCharacter.h"
#include "GameObject/PlayerBullet.h"
#include "GameObject/Reticle.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"
// State
#include "State/BaseCharacterState.h"
#include "State/PlayerRootState.h"
#include "State/PlayerDashState.h"
#include "State/PlayerAttackState.h"
// modelLoader
#include "Loader/modelLoader.h"

/// <summary>
/// Playerの部品
/// </summary>
enum PlayerParts {
	Parts_Body = 0,
	Parts_Face,
	Parts_LeftArm,
	Parts_RightArm
};

enum class PlayerBehavior {
	kRoot,		// 通常攻撃
	kAttack,	// 攻撃中,
	kDash,		// ダッシュ中
};

/// <summary>
/// Playerクラス
/// </summary>
class Player
	: public BaseCharacter {
public:

	Player(std::vector<Model*> models);
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::vector<Model*> models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) const override;

	/// <summary>
	/// 2Dのレティクルを描画する
	/// </summary>
	void Draw2DReticle(const bool& isLockOnMode);

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// Playerを動かす
	/// </summary>
	void Move();

	/// <summary>
	/// 弾の更新を行う
	/// </summary>
	void BulletsUpdate();

	/// <summary>
	///	弾をリストに追加する
	/// </summary>
	/// <param name="velocity"></param>
	void AddBulletList(const Vector3& velocity);

	/// <summary>
	///	状態を変更する
	/// </summary>
	/// <param name="behavior"></param>
	void ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior);
	
	/// <summary>
	/// Playerの状態を変化させるリクエストがあるかを確認する
	/// </summary>
	void CheckBehaviorRequest();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// ImGui
	/// </summary>
	void EditImGui();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void ApplyAdjustItems();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// world空間での座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const override;

	/// <summary>
	/// ViewProjectionのポインタを設定する
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	/// <summary>
	/// Playerの状態を変えるためのリクエストを送る
	/// </summary>
	/// <param name="request">変更したい状態</param>
	/// <param name="・">kRoot、kAttack、kDash</param>
	void SetBehaviorRequest(const PlayerBehavior& request) {
		behaviorRequest_ = request;
	}

	// ------------ Translation ------------ // 
	const Vector3 GetTranslation() const { return worldTransform_.translation_; }
	void SetTranslation(const Vector3& traslation) { worldTransform_.translation_ = traslation; }

	// ------------ PlayerのMatrix ------------ // 
	const Matrix4x4 GetMatWorld() const { return worldTransform_.matWorld_; }

	// ------------ Transform ------------ // 
	void SetParent(const WorldTransform* parent);
	void SetWorldTransform(const WorldTransform& worldTransform);
	void SetReticleParent(const WorldTransform* parent);
	
	// ------------ bulletList ------------ // 
	std::list<std::unique_ptr<PlayerBullet>>& GetPlayerBulletList() { return playerBulletList_; }

	// ------------ Reticle関連 ------------ // 
	Reticle* GetReticle() { return reticle_.get(); }
	const bool GetIsLockOnMode() const { return isLockOnMode_; }

private:

	const ViewProjection* viewProjection_ = nullptr;

	// ------------ 移動に関する変数 ------------ // 
	// 方向
	Vector3 direction_;
	// 速度
	Vector3 velocity_;

	// ------------ Playerの状態に関する変数 ------------ // 
	PlayerBehavior behavior_ = PlayerBehavior::kRoot;
	std::unique_ptr<BaseCharacterState> behaviorState_ = nullptr;
	std::optional<PlayerBehavior> behaviorRequest_ = std::nullopt;

	// ------------ Bulletに関する変数 ------------ // 
	// model
	Model* bullerModel_;

	// 通常弾のリスト
	std::list<std::unique_ptr<PlayerBullet>> playerBulletList_;

	// ------------ Reticleに関する変数 ------------ // 
	std::unique_ptr<Reticle> reticle_ = nullptr;

	// ------------ 単体で機能させる変数 ------------ // 
	bool isBossBattle_;

	bool isLockOnMode_;

	uint32_t hp_;
};
