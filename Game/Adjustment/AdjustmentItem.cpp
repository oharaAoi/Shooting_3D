#include "AdjustmentItem.h"

AdjustmentItem* AdjustmentItem::GetInstance() {
	static AdjustmentItem instance;
	return &instance;
}

void AdjustmentItem::Init() {
}

void AdjustmentItem::Update() {
	if (!ImGui::Begin("AdjustValue", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar()) { return; }

	// ------------------------------------------
	// 各グループを処理を行う
	// ------------------------------------------
	for (std::map<std::string, Group>::iterator itGroup = data_.begin(); itGroup != data_.end(); ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}
		// グループの参照を取得
		Group& group = itGroup->second;

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			if (std::holds_alternative<std::vector<Vector3>>(item.value)) {
				// `std::vector<Vector3>` を取り出す
				std::vector<Vector3> vec = std::get<std::vector<Vector3>>(item.value);
				// 要素がなかったら飛ばす
				if (vec.size() <= 0) {
					continue;
				}

				char buffer[256] = "";        // 文字列バッファを用意

				// elementIndexからバッファにコピー
				strncpy_s(buffer, sizeof(buffer), elementIndex_.c_str(), _TRUNCATE);
				// 要素を入力
				if (ImGui::InputText("point", buffer, sizeof(buffer))) {
					elementIndex_ = buffer;
				}
				// 空白でなければ入力されている要素を変更
				if (elementIndex_ != "") {
					if (std::stoi(elementIndex_) < vec.size()) {
						ImGui::SliderFloat3(itemName.c_str(), &vec[std::stoi(elementIndex_)].x, -10.0f, 10.0f);
					}
				}
			}
		}

		// 改行
		ImGui::Text("\n");

		// ボタン
		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "Adjustment", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void AdjustmentItem::CreateGroup(const std::string& groupName) {
	data_[groupName];
}

void AdjustmentItem::SaveFile(const std::string& groupName) {
	// グループ検索
	std::map<std::string, Group>::iterator itGroup = data_.find(groupName);
	// 未登録チェック
	assert(itGroup != data_.end());

	json root;
	root = json::object();
	// jsonオブジェクト登録
	root[groupName] = json::object();

	// ------------------------------------------
	// jsonに書き込む用の変数にデータを入れる
	// ------------------------------------------
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// Vector3を格納した配列を保持していれば
		if (std::holds_alternative<std::vector<Vector3>>(item.value)) {
			// Vector3の配列を取得
			const std::vector<Vector3>& vec = std::get<std::vector<Vector3>>(item.value);
			// json配列を作成
			json jsonArray = json::array();

			// 各Vector3をシリアライズして配列に追加
			for (const Vector3& vec3 : vec) {
				jsonArray.push_back(vec3.to_json());
			}

			// 配列をルートJSON構造に追加
			root[groupName][itemName] = jsonArray;
		}
	}

	// ------------------------------------------
	// ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(kDirectorPath);
	if (!std::filesystem::exists(kDirectorPath)) {
		std::filesystem::create_directories(kDirectorPath);
	}

	// ------------------------------------------
	// 書き込みを行う
	// ------------------------------------------
	// 書き込むJSONファイルのフルパス
	std::string filePath = kDirectorPath + groupName + ".json";
	// 書き込み用のファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (ofs.fail()) {
		std::string message = "Faild open data file for write";
		MessageBoxA(nullptr, message.c_str(), "Adjustment", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}
