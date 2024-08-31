#include "SceneManager.h"

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {}

void SceneManager::Finalize() {
	scene_.reset();
}

void SceneManager::Init() {
	audioManager_ = AudioManager::GetInstacne();
	audioManager_->Init();

	ModelLoader* modelLoader = ModelLoader::GetInstacne();
	modelLoader->Init();

	switch (nowScene_) {
	case Scene::kTitle:
		scene_ = std::make_unique<TitleScene>();
		scene_->Initialize();
		break;
	case Scene::kGame:
		scene_ = std::make_unique<GameScene>();
		scene_->Initialize();
		break;
	}
}

void SceneManager::Update() {
	audioManager_->Update();

	if (scene_->GetIsFinish()) {
		switch (nowScene_) {
		case Scene::kTitle:
			nowScene_ = Scene::kGame;
			scene_.reset(new GameScene);
			scene_->Initialize();
			break;
		case Scene::kGame:
			nowScene_ = Scene::kTitle;
			scene_.reset(new TitleScene);
			scene_->Initialize();
			break;
		}
	}

	scene_->Update();
}

void SceneManager::Draw() {
	scene_->Draw();

	audioManager_->IsPlay();
}
