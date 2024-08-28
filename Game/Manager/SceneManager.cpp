#include "SceneManager.h"

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}

void SceneManager::Init() {
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
	scene_->Update();

	if (scene_->GetIsFinish()) {
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
}

void SceneManager::Draw() {
	scene_->Draw();
}
