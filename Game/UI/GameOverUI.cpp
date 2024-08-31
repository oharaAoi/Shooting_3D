#include "GameOverUI.h"
#include "TextureManager.h"

GameOverUI::GameOverUI() {
	Init();
}

GameOverUI::~GameOverUI() {
}

void GameOverUI::Init() {
	gameOverTextureHandle_ = TextureManager::Load("./Resources/UI/GameOver/gameOver.png");
	pushButtonTextureHandle_ = TextureManager::Load("UI/Title/startButtonUI.png");

	gameOverColor_ = {1,1,1,1};
	pushButtonColor_ = { 1,1,1,1 };

	Sprite* gameOver = Sprite::Create(gameOverTextureHandle_, {640, 300}, gameOverColor_, {0.5f, 0.5f});
	Sprite* button = Sprite::Create(pushButtonTextureHandle_, {500, 480 }, pushButtonColor_, { 0.0f, 0.0f });

	gameOverSprite = std::unique_ptr<Sprite>(gameOver);
	pushButtonSprite = std::unique_ptr<Sprite>(button);
}

void GameOverUI::Update() {
}

void GameOverUI::Draw() {
	gameOverSprite->Draw();
	pushButtonSprite->Draw();
}
