#pragma once
#include <optional>
#include "GameObject/BaseCharacter.h"
#include "Adjustment/AdjustmentItem.h"

enum PlayerParts {
	Parts_Body = 0,
	Parts_Face,
	Parts_LeftArm,
	Parts_RightArm
};

class Player
	: public BaseCharacter {
public:

	Player(std::vector<Model*> models);
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::vector<Model*> models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	/// <summary>
	/// ImGui
	/// </summary>
	void EditImGui();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void ApplyAdjustItems();

private:

};

