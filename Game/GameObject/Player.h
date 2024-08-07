#pragma once
#include <optional>
#include "GameObject/BaseCharacter.h"
#include "Adjustment/AdjustmentItem.h"

/// <summary>
/// Playerの部品
/// </summary>
enum PlayerParts {
	Parts_Body = 0,
	Parts_Face,
	Parts_LeftArm,
	Parts_RightArm
};

/// <summary>
/// Playerクラス
/// </summary>
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
	/// Playerを動かす
	/// </summary>
	void Move();

	/// <summary>
	/// ImGui
	/// </summary>
	void EditImGui();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void ApplyAdjustItems();

///////////////////////////////////////////////////////////
// accessor
///////////////////////////////////////////////////////////

	/// <summary>
	/// ViewProjectionのポインタを設定する
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

	const ViewProjection* viewProjection_ = nullptr;

	// ------------ 移動に関する変数 ------------ // 
	// 方向
	Vector3 direction_;
	// 速度
	Vector3 velocity_;
};

