#pragma once
// STL
#include <memory>
// Scene
#include "GameScene.h"
#include "Scene/TitleScene.h"
#include "Scene/GameClearScene.h"
// Managet
#include "Manager/AudioManager.h"
// Loader
#include "Loader/ModelLoader.h"

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

	/// <summary>
	/// シーンを切り返る
	/// </summary>
	/// <param name="nextScene"></param>
	void ChangeScene(const Scene& nextScene);

private:

	std::unique_ptr<BaseScene> scene_ = nullptr;

	AudioManager* audioManager_ = nullptr;

};

