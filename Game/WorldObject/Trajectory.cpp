#include "Trajectory.h"

Trajectory::Trajectory() { Init(); }
Trajectory::~Trajectory() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trajectory::Init() {
	trajectoryType_.emplace(TrajectoryType::Player_Trajectory, "playerTrajectory");
	nowMoveTrajectoryType_ = TrajectoryType::Player_Trajectory;

	std::vector<Vector3> testArray;
	testArray.push_back({ 1.0f, 1.0f , 20.0f });
	testArray.push_back({ 10.0f, 300.0f , 20.0f });

	AdjustmentItem::GetInstance()->CreateGroup(trajectoryType_[nowMoveTrajectoryType_]);
	/*AdjustmentItem::GetInstance()->SetValue(trajectoryType_[nowMoveTrajectoryType_], "Point", testArray);*/

	playerTrajectory = AdjustmentItem::GetInstance()->GetValue<std::vector<Vector3>>("playerTrajectory", "Point");

	addPoint_ = { 0,0,0 };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trajectory::Update() {
	ImGuiDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trajectory::Draw() const {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ ImGuiの描画を管理する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Trajectory::ImGuiDraw() {
	ImGui::Begin("Trajectory");
	
	// ---------------------------------------------
	// ↓ 新しい点を追加する処理
	// ---------------------------------------------
	if (ImGui::BeginMenu("AddPoint")) {
		ImGui::SliderFloat3("addPoint", &addPoint_.x, -10.f, 10.0f);
		if (ImGui::Button("Add")) {
			playerTrajectory.push_back(addPoint_);
			AdjustmentItem::GetInstance()->SetValue(trajectoryType_[nowMoveTrajectoryType_], "Point", playerTrajectory);
		}
		ImGui::EndMenu();
	}

	// ---------------------------------------------
	// ↓ 点の位置を管理する
	// ---------------------------------------------
	if (ImGui::BeginMenu("ChangePoint")) {
		char buffer[256] = ""; // 文字列バッファを用意
		// elementIndexからバッファにコピー
		strncpy_s(buffer, sizeof(buffer), elementIndex_.c_str(), _TRUNCATE);
		// 要素を入力
		if (ImGui::InputText("point", buffer, sizeof(buffer))) {
			elementIndex_ = buffer;
		}
		// 空白でなければ入力されている要素を変更
		if (elementIndex_ != "") {
			if (std::stoi(elementIndex_) < playerTrajectory.size()) {
				ImGui::SliderFloat3("point", &playerTrajectory[std::stoi(elementIndex_)].x, -10.0f, 10.0f);
			}
		}
		ImGui::EndMenu();
	}
	ImGui::End();
}

