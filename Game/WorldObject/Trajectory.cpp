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
	
	// --------------------------------------
	// 円を描く点を設定する
	// --------------------------------------
	//float angle = 0.0f;
	Vector3 pos{};
	/*for (uint32_t oi = 0; oi < 121; ++oi) {
		pos.x = pos.x + std::cos(angle) * 3.0f;
		pos.y = pos.y + std::sin(angle) * 3.0f;
		pos.z = pos.z + std::sin(angle) * 3.0f;

		angle += std::numbers::pi_v<float> * kDeltaTime_;

		testArray.push_back(pos);
	}*/

	for (uint32_t oi = 0; oi < 121; ++oi) {
		pos.x = 0;
		pos.y = 0;
		pos.z += oi * 0.2f;

		testArray.push_back(pos);
	}

	// 最初と最後をつなぐ
	testArray.push_back(testArray[0]);

	AdjustmentItem::GetInstance()->CreateGroup(trajectoryType_[nowMoveTrajectoryType_]);
	AdjustmentItem::GetInstance()->SetValue(trajectoryType_[nowMoveTrajectoryType_], "Point", testArray);

	playerTrajectoryVector_ = AdjustmentItem::GetInstance()->GetValue<std::vector<Vector3>>("playerTrajectory", "Point");

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

	// --------------------------------------
	// Playerの軌道を描画する
	// --------------------------------------
	std::vector<Vector3> pointDrawing;
	// 点からSpline曲線を引く
	for (uint32_t oi = 0; oi < playerTrajectoryVector_.size(); ++oi) {
		float t = (1.0f / static_cast<float>(playerTrajectoryVector_.size())) * oi;
		Vector3 pos = CatmullRomPosition(playerTrajectoryVector_, t);
		pointDrawing.push_back(pos);
	}

	// 線を描画する
	for (size_t oi = 0; oi < pointDrawing.size() - 1; ++oi) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointDrawing[oi], pointDrawing[oi + 1], { 1.0f, 0.0f, 0.0f, 1.0f });
	}

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
			playerTrajectoryVector_.push_back(addPoint_);
			AdjustmentItem::GetInstance()->SetValue(trajectoryType_[nowMoveTrajectoryType_], "Point", playerTrajectoryVector_);
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
			if (std::stoi(elementIndex_) < playerTrajectoryVector_.size()) {
				ImGui::SliderFloat3("point", &playerTrajectoryVector_[std::stoi(elementIndex_)].x, -10.0f, 10.0f);
			}
		}
		ImGui::EndMenu();
	}
	ImGui::End();
}

