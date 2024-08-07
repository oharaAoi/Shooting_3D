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
		std::variant<uint32_t, float, Vector3, bool, std::vector<Vector3>> value;
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

	/// <summary>
	/// ファイル全体の読み込みを行う
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルの読み込みを行う(個別)
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// keyで指定した値をgroupに保存する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <param name="value">: 値</param>
	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);

	/// <summary>
	/// groupにkeyと値を追加する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <param name="value">: 値</param>
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& value);

	/// <summary>
	/// grounpからkeyで指定した値を取得する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <returns>: uint32_t, float, Vector3, bool, vector.Vector3 </returns>
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) const;

private:
	// 全データ
	std::map<std::string, Group> data_;

	std::string elementIndex_ = "0";
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
inline void AdjustmentItem::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = data_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
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