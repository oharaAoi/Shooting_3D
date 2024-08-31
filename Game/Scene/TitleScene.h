#pragma once
#include <memory>
// KamataEngine
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
// Scene
#include "Scene/BaseScene.h"
// WorldObject
#include "WorldObject/Skydome.h"
#include "WorldObject/Ground.h"
// Loader
#include "Loader/ModelLoader.h"
// Managet
#include "Manager/AudioManager.h"

class TitleScene
: public BaseScene {
public:

	TitleScene();
	~TitleScene() override;

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() override;

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ------------ View ------------ // 
	ViewProjection viewProjection_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Sprite> startButtonUI_;
	uint32_t startButtonUIHandle_;
	Vector2 startUIPos_;

	bool isPlayAudio_ = false;
};

