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
	case Scene::kClear:
		scene_ = std::make_unique<GameClearScene>();
		scene_->Initialize();
		break;
	}
}

void SceneManager::Update() {
	audioManager_->Update();

	if (scene_->GetIsFinish()) {
		ChangeScene(scene_->GetNextScene());
	}

	scene_->Update();
}

void SceneManager::Draw() {
	scene_->Draw();

	audioManager_->IsPlay();
}

void SceneManager::ChangeScene(const Scene& nextScene) {
	switch (nextScene) {
	case Scene::kTitle:
		scene_.reset(new TitleScene);
		scene_->Initialize();
		break;
	case Scene::kGame:
		scene_.reset(new GameScene);
		scene_->Initialize();
		break;
	case Scene::kClear:
		scene_.reset(new GameClearScene);
		scene_->Initialize();
		break;
	}
}
