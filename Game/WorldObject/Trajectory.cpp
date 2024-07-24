#include "Trajectory.h"

Trajectory::Trajectory() { Init(); }
Trajectory::~Trajectory() {}

//=============================================
// 初期化処理
//=============================================

void Trajectory::Init() {
	trajectoryType_.emplace(TrajectoryType::Player_Trajectory, "playerTrajectory");
	nowMoveTrajectoryType_ = TrajectoryType::Player_Trajectory;

	std::vector<Vector3> testArray;
	testArray.push_back({ 1.0f, 1.0f , 20.0f });
	testArray.push_back({ 10.0f, 300.0f , 20.0f });

	AdjustmentItem::GetInstance()->CreateGroup(trajectoryType_[nowMoveTrajectoryType_]);
	AdjustmentItem::GetInstance()->SetValue(trajectoryType_[nowMoveTrajectoryType_], "Point", testArray);
}

//=============================================
// 更新処理
//=============================================

void Trajectory::Update() {

}

//=============================================
// 描画処理
//=============================================

void Trajectory::Draw() {

}

