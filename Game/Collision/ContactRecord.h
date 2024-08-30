#pragma once
#include <vector>
#include <algorithm>

/// <summary>
/// 接触記録
/// </summary>
class ContactRecord {
public:

	ContactRecord();
	~ContactRecord();

	// 履歴追加
	void AddRecord(const uint32_t& number);

	// 履歴チェック
	bool CheckRecord(const uint32_t& number);

	// 履歴クリア
	void Clear();

private:

	std::vector<uint32_t> record_;

};;

