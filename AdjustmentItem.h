#pragma once
// STL
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <variant>
// math
#include "Math/MyMath.h"
// json
#include <json.hpp>

#ifdef _DEBUG
#include <ImGuiManager.h>
#endif // _DEBUG

using json = nlohmann::json;

// グローバル変数の保存先ファイルパス
const std::string kDirectorPath = "Resources/AdjustmentItem/";

/// <summary>
/// 調整項目を扱うクラス
/// </summary>
class AdjustmentItem {
public: // 構造体

	// 軌道の点
	struct Item {
		std::variant<std::vector<Vector3>> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

public:

	AdjustmentItem() = default;
	~AdjustmentItem() = default;

	// シングルトン化
	AdjustmentItem(const AdjustmentItem&) = delete;
	AdjustmentItem& operator=(const AdjustmentItem&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>AdjustmentItemクラスのインスタンス</returns>
	static AdjustmentItem* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルにセーブを行う
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);

	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& t);

	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) const;

private:
	// 全データ
	std::map<std::string, Group> data_;
};

template<typename T>
inline void AdjustmentItem::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	// グループの参照を取得
	Group& group = data_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void AdjustmentItem::AddItem(const std::string& groupName, const std::string& key, const T& t) {
	Group& group = data_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, t);
	}
}

template<typename T>
inline T AdjustmentItem::GetValue(const std::string& groupName, const std::string& key) const {
	// 未登録チェック
	assert(data_.find(groupName) != data_.end());
	// グループの参照を取得
	const Group& group = data_.at(groupName);

	assert(group.items.find(key) != group.items.end());
	const Item& item = group.items.at(key);

	return std::get<T>(item.value);
}