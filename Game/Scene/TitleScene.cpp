#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	ModelLoader* modelLoader = ModelLoader::GetInstacne();

	skydome_ = std::make_unique<Skydome>(modelLoader->GetModel("skydome"));
	ground_ = std::make_unique<Ground>(modelLoader->GetModel("ground"));

	startUIPos_ = { 640, 555 };
	startButtonUIHandle_ = TextureManager::Load("UI/Title/startButtonUI.png");
	Sprite* statUI = Sprite::Create(startButtonUIHandle_, startUIPos_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f });
	startButtonUI_ = std::unique_ptr<Sprite>(statUI);

	isPlayAudio_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Update() {
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }
	Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		isFinish_ = true;
		AudioManager::GetInstacne()->StopAudioPlayerList("Audio/title.wav");
		AudioManager::GetInstacne()->AddPlayList("Audio/pushButton.wav", false, 0.7f);
	}

	skydome_->Update();
	ground_->Update();

	startButtonUI_->SetPosition(startUIPos_);

	viewProjection_.TransferMatrix();

	if (!isPlayAudio_) {
		AudioManager::GetInstacne()->AddPlayList("Audio/title.wav", true, 0.5f);
		isPlayAudio_ = true;
	}

#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::SliderFloat2("startUIPos", &startUIPos_.x, 0, 1280);
	ImGui::End();
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Draw() {

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
	
	startButtonUI_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
