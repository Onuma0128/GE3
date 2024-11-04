#pragma once

#include <variant>
#include <map>
#include <string>

#include "Vector3.h"

class GlobalVariables
{
private:
	static GlobalVariables* instance_;

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(GlobalVariables&) = delete;
	GlobalVariables& operator=(GlobalVariables&) = delete;

	// 項目
	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};
	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};
public:
	// グループの作成
	void CreateGroup(const std::string& groupName);
	// 項目のセッター
	template <typename T>
	void SetValue(const std::string& groupName, const std::string& key, T value);

public:
	// シングルトンインスタンスの取得
	static GlobalVariables* GetInstance();
	// 全データ
	std::map<std::string, Group> datas_;
	// 更新
	void Update();

};

template<typename T>
inline void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, T value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを追加
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
