#pragma once
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
	void Draw();

private:

	// --------- プレイヤーの軌道　--------- //
	std::vector<Vector3> playerTrajectory;

	// --------- 軌道の種類　--------- //
	std::unordered_map<TrajectoryType, std::string> trajectoryType_;
	TrajectoryType nowMoveTrajectoryType_;
	
};

