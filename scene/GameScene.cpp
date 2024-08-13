#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	PrimitiveDrawer::GetInstance()->Initialize();

	viewProjection_.Initialize();

	// ---------------------------------------------
	// ↓ Loaderの初期化
	// ---------------------------------------------
	ModelLoader* modelLoader = ModelLoader::GetInstacne();
	modelLoader->Init();

	AdjustmentItem::GetInstance()->Init();

	// ---------------------------------------------
	// ↓ Cameraの初期化
	// ---------------------------------------------
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	followCamera_ = std::make_unique<FollowCamera>();

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// ---------------------------------------------
	// ↓ GameObjectの初期化
	// ---------------------------------------------

	// player ---------------------------------------------------------
	// 使用するモデルを配列に格納
	playerPartsModels_.emplace_back(modelLoader->GetModel("player_body"));
	playerPartsModels_.emplace_back(modelLoader->GetModel("player_face"));
	playerPartsModels_.emplace_back(modelLoader->GetModel("player_leftArm"));
	playerPartsModels_.emplace_back(modelLoader->GetModel("player_rightArm"));

	// インスタンス生成と初期化
	player_ = std::make_unique<Player>(playerPartsModels_);

	player_->SetViewProjection(&viewProjection_);
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// enemy ---------------------------------------------------------
	mobEnemyPartsModels_.emplace_back(modelLoader->GetModel("mobEnemy"));

	// インスタンス生成と初期化
	std::unique_ptr<MobEnemy> mobEnemy = std::make_unique<MobEnemy>(mobEnemyPartsModels_);
	mobEnemyList_.emplace_back(std::move(mobEnemy));

	// ---------------------------------------------
	// ↓ WorldObjectの初期化
	// ---------------------------------------------
	skydome_ = std::make_unique<Skydome>(modelLoader->GetModel("skydome"));
	trajectory_ = std::make_unique<Trajectory>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update() {

	// ---------------------------------------------
	// ↓ GameObjectの処理
	// ---------------------------------------------
	player_->Update();

	// enmey
	for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemyList_) {
		mobEnemy->Update();
	}

	// ---------------------------------------------
	// ↓ WorldObjectの処理
	// ---------------------------------------------
	skydome_->Update();
	trajectory_->Update();

	// ---------------------------------------------
	// ↓ Cameraの処理
	// ---------------------------------------------
	if (input_->TriggerKey(DIK_C)) {
		isDebugCamera = !isDebugCamera;
	}

	if (isDebugCamera) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		AxisIndicator::GetInstance()->SetVisible(true);
	} else {
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		AxisIndicator::GetInstance()->SetVisible(false);
	}

	viewProjection_.TransferMatrix();

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// ---------------------------------------------
	// ↓ ImGuiの処理
	// ---------------------------------------------
#ifdef _DEBUG
	AdjustmentItem::GetInstance()->Update();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	// ---------------------------------------------
	// ↓ WorldObjectの描画
	// ---------------------------------------------
	skydome_->Draw(viewProjection_);

	// ---------------------------------------------
	// ↓ GameObjectの描画
	// ---------------------------------------------
	player_->Draw(viewProjection_);

	// enemy
	for (const std::unique_ptr<MobEnemy>& mobEnemy : mobEnemyList_) {
		mobEnemy->Draw(viewProjection_);
	}

	// ---------------------------------------------
	// ↓ 線の描画
	// ---------------------------------------------
	trajectory_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
