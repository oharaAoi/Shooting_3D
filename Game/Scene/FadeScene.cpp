#include "FadeScene.h"
#include "TextureManager.h"

FadeScene::FadeScene(const FadeInOutType& inOut) {
	Init(inOut);
}

FadeScene::~FadeScene() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadeScene::Init(const FadeInOutType& inOut) {
	inOutType_ = inOut;
	
	// ---------------------------------------------
	// ↓ blockPanel
	// ---------------------------------------------
	blackPanelHandle_ = TextureManager::Load("./Resources/Fade/BlackPanel.png");
	blackPanelColor_ = { 1,1,1,1 };
	Sprite* blackPanel = Sprite::Create(blackPanelHandle_, { 640, 360 }, blackPanelColor_, { 0.5f, 0.5f });
	blackPanel_ = std::unique_ptr<Sprite>(blackPanel);
	blackFadeCount_ = 0;
	blackFadeTime_ = 150;

	// ---------------------------------------------
	// ↓ whitePanel
	// ---------------------------------------------
	whitePanelHandle_ = TextureManager::Load("./Resources/Fade/whitePanel.png");
	whitePanelColor_ = { 1,1,1,1 };
	Sprite* whitePanel = Sprite::Create(whitePanelHandle_, { 640, 360 }, whitePanelColor_, { 0.5f, 0.5f });
	whitePanel_ = std::unique_ptr<Sprite>(whitePanel);
	whiteFadeCount_ = 0;
	whiteFadeTime_ = 120;

	// ---------------------------------------------
	// ↓ bool
	// ---------------------------------------------
	isFade_ = false;
	isFadeFinish_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadeScene::Update() {
	switch (fadeType_) {
	case None_Fade:

		break;
	case BlackInOut_Fade:
		BlackInOut();
		break;
	case WhiteInOut_Fade:
		WhiteInOut();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadeScene::Draw() {
	switch (fadeType_) {
	case None_Fade:

		break;
	case BlackInOut_Fade:
		blackPanel_->Draw();
		break;
	case WhiteInOut_Fade:
		whitePanel_->Draw();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　BlackInOut
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadeScene::BlackInOut() {
	float t = 0;
	float easeValue = 0;
	switch (inOutType_) {
	case FadeIn_Type:
		blackFadeCount_++;
		t = static_cast<float>(blackFadeCount_) / static_cast<float>(blackFadeTime_);
		easeValue = std::lerp(0.0f, 1.0f, t);
		blackPanelColor_.w = easeValue;

		if (blackFadeCount_ >= blackFadeTime_) {
			isFadeFinish_ = true;
			blackFadeCount_ = 0;
		}
		blackPanel_->SetColor(blackPanelColor_);

		break;
	case FadeOut_Type:
		blackFadeCount_++;
		t = static_cast<float>(blackFadeCount_) / static_cast<float>(blackFadeTime_);
		easeValue = std::lerp(1.0f, 0.0f, t);
		blackPanelColor_.w = easeValue;
		if (blackFadeCount_ >= blackFadeTime_) {
			isFadeFinish_ = true;
			blackFadeCount_ = 0;
		}

		blackPanel_->SetColor(blackPanelColor_);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　WhiteInOut
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadeScene::WhiteInOut() {
	float t = 0;
	float easeValue = 0;
	switch (inOutType_) {
	case FadeIn_Type:
		whiteFadeCount_++;
		t = static_cast<float>(whiteFadeCount_) / static_cast<float>(whiteFadeTime_);
		easeValue = std::lerp(0.0f, 1.0f, t);
		whitePanelColor_.w = easeValue;

		if (whiteFadeCount_ >= whiteFadeTime_) {
			isFadeFinish_ = true;
			whiteFadeCount_ = 0;
		}
		whitePanel_->SetColor(whitePanelColor_);

		break;
	case FadeOut_Type:
		whiteFadeCount_++;
		t = static_cast<float>(whiteFadeCount_) / static_cast<float>(whiteFadeTime_);
		easeValue = std::lerp(1.0f, 0.0f, t);
		whitePanelColor_.w = easeValue;
		if (whiteFadeCount_ >= whiteFadeTime_) {
			isFadeFinish_ = true;
			whiteFadeCount_ = 0;
		}

		whitePanel_->SetColor(whitePanelColor_);
		break;
	}
}
