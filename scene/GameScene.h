#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
// Scene
#include "Scene/BaseScene.h"
#include "Scene/FadeScene.h"
// Camera
#include "DebugCamera.h"
#include "Camera/FollowCamera.h"
#include "Camera/RailCamera.h"
// Loader
#include "Loader/modelLoader.h"
// WorldObject
#include "WorldObject/Skydome.h"
#include "WorldObject/Trajectory.h"
#include "WorldObject/Ground.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"
// GameObject
#include "GameObject/Player.h"
#include "GameObject/MobEnemy.h"
#include "GameObject/EnemyBullet.h"
#include "GameObject/BossBullet.h"
#include "GameObject/Reticle.h"
#include "GameObject/RecoverItem.h"
// Manager
#include "Collision/CollisionManager.h"
#include "Manager/EnemyManager.h"
// UI
#include "UI/PlayerUI.h"
#include "UI/Rader.h"
#include "UI/BossUI.h"
#include "UI/GameOverUI.h"
// Effect
#include "Effect/EmissionEffect.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
 : public BaseScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

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

	/// <summary>
	/// ゲームオーバー時に行う処理
	/// </summary>
	void GameOverUpdate();

	/// <summary>
	/// ゲームクリア時に行う処理
	/// </summary>
	void GameClearUpdate();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="origine"></param>
	/// <param name="lifeTime"></param>
	/// <param name="count"></param>
	void AddEmissionEffect(const Vector3& origine, const uint32_t& lifeTime, const uint32_t& count);

	void AddRecoverItem(const Vector3& pos);

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
	
	std::unique_ptr<FadeScene> fadeScene_ = nullptr;
	
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

	std::list<std::unique_ptr<RecoverItem>> recoverItemList_;
	Model* recoverModel_;

	// ------------ WorldEnvironment ------------ // 
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Trajectory> trajectory_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;

	// ------------ Collision ------------ // 
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ------------ Manager ------------ // 
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	// ------------ UI ------------ // 
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;
	std::unique_ptr<Rader> rader_ = nullptr;
	std::unique_ptr<BossUI> bossUI_ = nullptr;
	std::unique_ptr<Sprite> controlUI_ = nullptr;
	std::unique_ptr<GameOverUI> gameOverUI_ = nullptr;

	uint32_t controlUIHandle_;
	Vector2 controlUIPos_;

	// ------------ Particle ------------ // 
	std::unique_ptr<EmissionEffect> emissionEffect_ = nullptr;

	// ------------ メンバ変数 ------------ // 
	uint32_t playerAimCount_;
};
