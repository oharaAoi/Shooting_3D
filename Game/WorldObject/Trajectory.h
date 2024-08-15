#pragma once
#include "Enviroment.h"
#include "PrimitiveDrawer.h"
// STL
#include <vector>
#include <unordered_map>
#include <string>
// math
#include "Math/MyMath.h"
// adjustment
#include "Adjustment/AdjustmentItem.h"

#ifdef _DEBUG
#include <ImGuiManager.h>
#endif // _DEBUG

/// <summary>
/// 軌道の種類
/// </summary>
enum class TrajectoryType {
	Player_Trajectory,		// プレイヤー
};

/// <summary>
/// 軌道を管理するクラス
/// </summary>
class Trajectory {
public:

	Trajectory();
	~Trajectory();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() const;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void ImGuiDraw();

///////////////////////////////////////////////////////////
// メンバ関数
///////////////////////////////////////////////////////////

	std::vector<Vector3> GetPlayerTrajectoryVector() { return playerTrajectoryVector_; }

private:

	// --------- プレイヤーの軌道　--------- //
	std::vector<Vector3> playerTrajectoryVector_;

	// --------- 軌道の種類　--------- //
	std::unordered_map<TrajectoryType, std::string> trajectoryType_;
	TrajectoryType nowMoveTrajectoryType_;

	// --------- ImGuiで使用する変数　--------- //
	Vector3 addPoint_;
	// 要素名
	std::string elementIndex_ = "0";
};

