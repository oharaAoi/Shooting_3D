#include "GameClearUI.h"

GameClearUI::GameClearUI() {
	Init();
}

GameClearUI::~GameClearUI() {
}

void GameClearUI::Init() {
	gameClearTextureHandle_ = TextureManager::Load("./Resources/UI/gameClear.png");
	pushButtonTextureHandle_ = TextureManager::Load("UI/Title/startButtonUI.png");

	gameClearColor_ = { 1,1,1,1 };
	pushButtonColor_ = { 1,1,1,1 };

	Sprite* gameClear = Sprite::Create(gameClearTextureHandle_, { 640, 300 }, gameClearColor_, { 0.5f, 0.5f });
	Sprite* button = Sprite::Create(pushButtonTextureHandle_, { 500, 480 }, pushButtonColor_, { 0.0f, 0.0f });

	gameClearSprite = std::unique_ptr<Sprite>(gameClear);
	pushButtonSprite = std::unique_ptr<Sprite>(button);
}

void GameClearUI::Update() {
}

void GameClearUI::Draw() {
	gameClearSprite->Draw();
	pushButtonSprite->Draw();
}
