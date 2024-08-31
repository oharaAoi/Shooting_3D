#include "GameClearScene.h"

GameClearScene::GameClearScene() {
}

GameClearScene::~GameClearScene() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	ModelLoader* modelLoader = ModelLoader::GetInstacne();

	skydome_ = std::make_unique<Skydome>(modelLoader->GetModel("skydome"));
	ground_ = std::make_unique<Ground>(modelLoader->GetModel("ground"));

	fadeScene_ = std::make_unique<FadeScene>(FadeOut_Type);
	fadeScene_->SetIsFade(true);
	fadeScene_->SetFadeType(WhiteInOut_Fade);

	gameClearUI_ = std::make_unique<GameClearUI>();

	AudioManager::GetInstacne()->AddPlayList("Audio/gameClear.wav", false, 0.5f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameClearScene::Update() {
	skydome_->Update();
	ground_->Update();

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }
	Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		isFinish_ = true;
		nextScene_ = Scene::kTitle;
		AudioManager::GetInstacne()->AddPlayList("Audio/pushButton.wav", false, 0.7f);
	}

	if (!fadeScene_->GetIsFadeFinish()) {
		if (fadeScene_->GetIsFade()) {
			fadeScene_->Update();
		}
	}

	gameClearUI_->Update();

	viewProjection_.TransferMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameClearScene::Draw() {

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
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	gameClearUI_->Draw();

	fadeScene_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
