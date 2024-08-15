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
// Manager
#include "Collision/CollisionManager.h"

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
	/// すべての当たり判定を実行する
	/// </summary>
	void CheckAllCollision();

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
	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	// ------------ Loader ------------ // 
	std::unique_ptr<ModelLoader> modelLoader_ = nullptr;

	// ------------ GameObject ------------ // 
	// player
	std::vector<Model*> playerPartsModels_;
	std::unique_ptr<Player> player_ = nullptr;

	// mobEnemy
	std::vector<Model*> mobEnemyPartsModels_;
	std::list<std::unique_ptr<MobEnemy>> mobEnemyList_;

	// ------------ WorldEnvironment ------------ // 
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Trajectory> trajectory_ = nullptr;

	// ------------ Collision ------------ // 
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ------------ メンバ変数 ------------ // 
	bool isBossBattle_ = false;
};
