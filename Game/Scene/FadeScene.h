#pragma once
// STL
#include <memory>
// KamataEngine
#include "Sprite.h"
// Math
#include "Math/MyMath.h"
// Manager
#include "Manager/AudioManager.h"

enum FadeType {
	None_Fade,
	BlackInOut_Fade,
	WhiteInOut_Fade
};

enum FadeInOutType {
	FadeIn_Type,
	FadeOut_Type
};

class FadeScene {
public:

	FadeScene(const FadeInOutType& inOut);
	~FadeScene();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(const FadeInOutType& inOut);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	void SetFadeType(const FadeType& fadeType) {
		fadeType_ = fadeType;
	}

	void SetIsFade(const bool& isFade) { isFade_ = isFade; }
	bool GetIsFade() const { return isFade_; }

	void SetIsFadeFinish(const bool& isFinish) { isFadeFinish_ = isFinish; }
	bool GetIsFadeFinish() const { return isFadeFinish_; }

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

	void BlackInOut();

	void WhiteInOut();

private:

	FadeInOutType inOutType_;

	bool isFade_;
	bool isFadeFinish_;
	FadeType fadeType_;

	// ------------- BlockInOut  ------------- //
	std::unique_ptr<Sprite> blackPanel_;
	uint32_t blackPanelHandle_;
	Vector4 blackPanelColor_;

	uint32_t blackFadeCount_;
	uint32_t blackFadeTime_;

	// ------------- WhiteOut  ------------- //
	std::unique_ptr<Sprite> whitePanel_;
	uint32_t whitePanelHandle_;
	Vector4 whitePanelColor_;

	uint32_t whiteFadeCount_;
	uint32_t whiteFadeTime_;
};

