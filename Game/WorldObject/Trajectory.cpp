#include "Trajectory.h"

Trajectory::Trajectory() {}
Trajectory::~Trajectory() {}

//=============================================
// 初期化処理
//=============================================

void Trajectory::Init() {
	trajectoryType_.emplace(TrajectoryType::Player_Trajectory, "player");
	nowMoveTrajectoryType_ = TrajectoryType::Player_Trajectory;
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

