#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
// GameObject
#include "WorldObject/Skydome.h"
// Loader
#include "Loder/modelLoader.h"
// WorldObject
#include "WorldObject/Skydome.h"
#include "WorldObject/Trajectory.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"

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

	// ------------ Loader ------------ // 
	std::unique_ptr<ModelLoader> modelLoader_ = nullptr;

	// ------------ WorldEnvironment ------------ // 
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Trajectory> trajectory_ = nullptr;

};
