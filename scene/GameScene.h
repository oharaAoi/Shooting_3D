#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
// Camera
#include "DebugCamera.h"
#include "Camera/FollowCamera.h"
#include "Camera/RailCamera.h"
// GameObject
#include "WorldObject/Skydome.h"
// Loader
#include "Loader/modelLoader.h"
// WorldObject
#include "WorldObject/Skydome.h"
#include "WorldObject/Trajectory.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"
// GameObject
#include "GameObject/Player.h"
#include "GameObject/MobEnemy.h"
#include "GameObject/EnemyBullet.h"
#include "GameObject/BossBullet.h"
#include "GameObject/Reticle.h"
// Manager
#include "Collision/CollisionManager.h"
#include "Manager/EnemyManager.h"
// UI
#include "UI/PlayerUI.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// Game上のViewProjectionを更新する
	/// </summary>
	void UpdateViewProjection();

	/// <summary>
	/// 弾の更新を行う
	/// </summary>
	void UpdateBullet();

	/// <summary>
	/// すべての当たり判定を実行する
	/// </summary>
	void CheckAllCollision();
	
	/// <summary>
	/// 敵を生成する関数
	/// </summary>
	void EnemyPop();

	/// <summary>
	/// 敵弾をリストに追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	/// <summary>
	/// ボスの弾をリストに追加する
	/// </summary>
	/// <param name="bossBullet"></param>
	void AddBossBullet(std::unique_ptr<BossBullet> bossBullet);

	/// <summary>
	/// reticleと敵の当たり判定を行う
	/// </summary>
	void CheckCanLockOnEnemy();

	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	void EditImGui();

	void DrawGrid();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// ------------ View ------------ // 
	ViewProjection viewProjection_;

	// ------------ Camera ------------ // 
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCamera = false;

	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// ------------ Loader ------------ // 
	std::unique_ptr<ModelLoader> modelLoader_ = nullptr;

	// ------------ GameObject ------------ // 
	// player
	std::vector<Model*> playerPartsModels_;
	std::unique_ptr<Player> player_ = nullptr;

	 // reticle
	std::unique_ptr<Reticle> reticle_ = nullptr;

	// enemy
	std::list<std::unique_ptr<EnemyBullet>> enemyBulletList_;
	std::list<std::unique_ptr<BossBullet>> bossBulletList_;

	// ------------ WorldEnvironment ------------ // 
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Trajectory> trajectory_ = nullptr;

	// ------------ Collision ------------ // 
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ------------ Manager ------------ // 
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	// ------------ UI ------------ // 
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;

	// ------------ メンバ変数 ------------ // 
};
