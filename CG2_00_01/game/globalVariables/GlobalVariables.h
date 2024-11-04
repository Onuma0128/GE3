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
	// シングルトンインスタンスの取得
	static GlobalVariables* GetInstance();
	
	// 全データ
	std::map<std::string, Group> datas_;

};