#pragma once
// STL
#include <vector>
// KamataEngien
#include "Sprite.h"
#include "TextureManager.h"
// Math
#include "Math/MyMath.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class PlayerUI {
public:

	PlayerUI();
	~PlayerUI();

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
	void Draw() const;

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////


	void EditImGui();

private:


	std::unique_ptr<Sprite> hp_outside_;
	std::unique_ptr<Sprite> hp_bar_;

	uint32_t hp_outsideTextureHandle_;
	uint32_t hp_barTextureHandle_;

	Vector2 uiPosition_;
};

