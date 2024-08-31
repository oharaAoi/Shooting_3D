#pragma once
// STL
#include <memory>
// Scene
#include "GameScene.h"
#include "Scene/TitleScene.h"
// Managet
#include "Manager/AudioManager.h"
// Loader
#include "Loader/ModelLoader.h"

enum class Scene {
	kTitle,
	kGame
};

class SceneManager {
public:

	// 現在のシーン
	Scene nowScene_ = Scene::kTitle;

public:

	SceneManager();
	~SceneManager();

	void Finalize();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

private:

	std::unique_ptr<BaseScene> scene_ = nullptr;

	AudioManager* audioManager_ = nullptr;

};

