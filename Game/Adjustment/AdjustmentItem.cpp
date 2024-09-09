#include "AdjustmentItem.h"

AdjustmentItem* AdjustmentItem::GetInstance() {
	static AdjustmentItem instance;
	return &instance;
}

void AdjustmentItem::Init() {
	LoadFiles();
}

void AdjustmentItem::Update() {
#ifdef _DEBUG
	if (!ImGui::Begin("AdjustValue", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar()) { return; }

	// ------------------------------------------
	// ↓ 各グループを処理を行う
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

			// int32_t型の値を保持していれば
			if (std::holds_alternative<uint32_t>(item.value)) {
				int32_t* ptr = reinterpret_cast<int32_t*>(std::get_if<uint32_t>(&item.value));
				ImGui::DragInt(itemName.c_str(), ptr, 0, 100);

				// float型の値を保持していれば
			} else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::DragFloat(itemName.c_str(), ptr, 0, 100);

				// Vector3の値を保持していれば
			} else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);

				// bool型の値を保持していれば
			} else if (std::holds_alternative<bool>(item.value)) {
				bool* ptr = reinterpret_cast<bool*>(std::get_if<bool>(&item.value));
				ImGui::Checkbox(itemName.c_str(), ptr);

				// 配列型を持っていたら
			}else if (std::holds_alternative<std::vector<Vector3>>(item.value)) {
				// std::vector<Vector3>を取り出す
				std::vector<Vector3> vec = std::get<std::vector<Vector3>>(item.value);
				// 要素がなかったら飛ばす
				if (vec.size() <= 0) {
					continue;
				}

				char buffer[256] = ""; // 文字列バッファを用意
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
#endif // _DEBUG
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
	// ↓ jsonに書き込む用の変数にデータを入れる
	// ------------------------------------------
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<uint32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<uint32_t>(item.value);

			// float型
		} else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);

			// Vector3型
		} else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x, value.y, value.z });

			// bool
		} else if (std::holds_alternative<bool>(item.value)) {
			root[groupName][itemName] = std::get<bool>(item.value);

			// Vector3を格納した配列を保持していれば
		}else if (std::holds_alternative<std::vector<Vector3>>(item.value)) {
			// Vector3の配列を取得
			const std::vector<Vector3>& vec = std::get<std::vector<Vector3>>(item.value);
			// json配列を作成
			json jsonArray = json::array();

			// 各Vector3をシリアライズして配列に追加
			for (const Vector3& vec3 : vec) {
				to_json(jsonArray, vec3);
				jsonArray.push_back(vec3);
			}

			// 配列をルートJSON構造に追加
			root[groupName][itemName] = jsonArray;
		}
	}

	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(kDirectorPath);
	if (!std::filesystem::exists(kDirectorPath)) {
		std::filesystem::create_directories(kDirectorPath);
	}

	// ------------------------------------------
	// ↓ 書き込みを行う
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
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ディレクトリの全ファイル読み込み
/////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::LoadFiles() {
	// ディレクトリがなかったらスキップ
	std::filesystem::path dire(kDirectorPath);
	if (!std::filesystem::exists(kDirectorPath)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectorPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();
		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}
		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// グループごとでファイルを読み込む
/////////////////////////////////////////////////////////////////////////////////////////////

void AdjustmentItem::LoadFile(const std::string& groupName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectorPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + groupName + ".json";
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループがあるかを検索
	json::iterator itGroup = root.find(groupName);
	// 未登録チェック
	assert(itGroup != root.end());

	// 各項目についての処理
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		const std::string& itemName = itItem.key();
		// アイテム名を取得
		
		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を取得
			uint32_t value = itItem->get<uint32_t>();
			SetValue(groupName, itemName, value);

			// float型
		} else if (itItem->is_number_float()) {
			// float型の値を取得
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));

			// Vector3
		} else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);

			// bool
		} else if (itItem->is_boolean()) {
			bool flag = itItem->get<bool>();
			SetValue(groupName, itemName, flag);

			// 配列型の場合
		}else if (itItem->is_array()) {
			// 配列の要素がオブジェクトである場合、std::vector<Vector3>として処理
			if ((*itItem)[0].is_object()) {
				std::vector<Vector3>vectors = itItem->get<std::vector<Vector3>>();
				SetValue(groupName, itemName, vectors);
			}
		}
	}
}
