#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

//=============================================
// 初期化処理
//=============================================

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	// ---------------------------------------------
	// ↓ Loaderの初期化
	// ---------------------------------------------
	modelLoader_ = std::make_unique<ModelLoader>();

	// ---------------------------------------------
	// ↓ Cameraの初期化
	// ---------------------------------------------
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	// ---------------------------------------------
	// ↓ GameObjectの初期化
	// ---------------------------------------------
	skydome_ = std::make_unique<Skydome>(modelLoader_->GetModel("skydome"));
}

//=============================================
// 更新処理
//=============================================

void GameScene::Update() {

	skydome_->Update();

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
	}

	viewProjection_.TransferMatrix();
}

//=============================================
// 描画処理
//=============================================

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
	
	skydome_->Draw(viewProjection_);

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
