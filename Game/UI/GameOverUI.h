#pragma once
// STL
#include <vector>
// KamataEngien
#include "Sprite.h"
#include "TextureManager.h"
// Math
#include "Math/MyMath.h"
// Adjustment
#include "Adjustment/AdjustmentItem.h"

/// <summary>
/// GameOver時のSprite
/// </summary>
class GameOverUI {
public:

	GameOverUI();
	~GameOverUI();

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

	std::unique_ptr<Sprite> gameOverSprite;
	std::unique_ptr<Sprite> pushButtonSprite;

	uint32_t gameOverTextureHandle_;
	uint32_t pushButtonTextureHandle_;

	Vector4 gameOverColor_;
	Vector4 pushButtonColor_;

};

