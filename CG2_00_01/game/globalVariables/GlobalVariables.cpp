#include "GlobalVariables.h"

#include <windows.h>
#include <filesystem>
#include <fstream>

#include "imgui.h"

GlobalVariables* GlobalVariables::instance_ = nullptr;

void GlobalVariables::SaveFile(const std::string& groupName)
{
    // グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
    // 未登録チェック
    assert(itGroup != datas_.end());

    // グループの登録確認
    json root;
    root = json::object();
    // jsonオブジェクトの登録
    root[groupName] = json::object();
    
    // 各項目
    for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
        itItem != itGroup->second.end(); ++itItem) {
        // 項目名の取得
        const std::string& itemName = itItem->first;
        // 項目の参照を取得
        Item& item = itItem->second;

        // int32_t
        if (std::holds_alternative<int32_t>(item)) {
            root[groupName][itemName] = std::get<int32_t>(item);
        }
        // float
        else if (std::holds_alternative<float>(item)) {
            root[groupName][itemName] = std::get<float>(item);
        }
        // Vector3
        else if (std::holds_alternative<Vector3>(item)) {
            Vector3 value = std::get<Vector3>(item);
            root[groupName][itemName] = json::array({ value.x,value.y,value.z });
        }
    }
    // ファイル書き出し
    OutputToFile(groupName, root);
}

void GlobalVariables::OutputToFile(const std::string& groupName, json root)
{
    // ディレクトリが無ければ作成
    std::filesystem::path dir(kDirectoryPath);
    if (!std::filesystem::exists(kDirectoryPath)) {
        std::filesystem::create_directory(kDirectoryPath);
    }
    // 書き込むjsonファイルのフルパスを作成
    std::string filePath = kDirectoryPath + groupName + ".json";
    // 書き込み用ファイルストリーム
    std::ofstream ofs;
    // ファイルを書き込み用に開く
    ofs.open(filePath);

    // ファイルオープンに失敗
    if (ofs.fail()) {
        std::string message = "Failed open data fail for write.";
        MessageBoxA(nullptr, message.c_str(), "GrobalVariables", 0);
        assert(0);
        return;
    }

    // ファイルにjson文字列を書き込む
    ofs << std::setw(4) << root << std::endl;
    // ファイルを閉じる
    ofs.close();
}

void GlobalVariables::LoadFiles()
{
    // ディレクトリを確認
    std::filesystem::path dir(kDirectoryPath);
    if (!std::filesystem::exists(kDirectoryPath)) {
        return;
    }
    std::filesystem::directory_iterator dir_it(kDirectoryPath);
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

void GlobalVariables::LoadFile(const std::string& groupName)
{
    // 読み込むjsonファイルのフルパスを作成
    std::string filePath = kDirectoryPath + groupName + ".json";
    // 読み込み用ファイルストリーム
    std::ifstream ifs;
    // ファイルを読み込み用に開く
    ifs.open(filePath);

    // ファイルオープンに失敗
    if (ifs.fail()) {
        std::string message = "Failed to open file for writing.";
        MessageBoxA(nullptr, message.c_str(), "GrobalVariables", 0);
        assert(0);
        return;
    }

    json root;
    // json文字列からjsonのデータ構造に展開
    ifs >> root;
    // ファイルを閉じる
    ifs.close();

    // グループを検索
    json::iterator itGroup = root.find(groupName);
    // 未登録チェック
    assert(itGroup != root.end());

    // 各アイテム
    for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
        // アイテム名を取得
        const std::string& itemName = itItem.key();
        // int32_t
        if (itItem->is_number_integer()) {
            int32_t value = itItem->get<int32_t>();
            SetValue(groupName, itemName, value);
        }
        // float
        else if (itItem->is_number_float()) {
            double value = itItem->get<double>();
            SetValue(groupName, itemName, static_cast<float>(value));
        }
        // Vector3
        else if (itItem->is_array() && itItem->size() == 3) {
            Vector3 value = { itItem->at(0),itItem->at(1), itItem->at(2) };
            SetValue(groupName, itemName, value);
        }
    }
}

GlobalVariables* GlobalVariables::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new GlobalVariables;
    }
    return instance_;
}

void GlobalVariables::Update()
{
    if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }
    if (!ImGui::BeginMenuBar()) return;

    // 各グループ
    for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
        itGroup != datas_.end(); ++itGroup) {
        // グループ名を取得
        const std::string& groupName = itGroup->first;
        // グループの参照を取得
        Group& group = itGroup->second;
        // グループのメニューを追加
        if (!ImGui::BeginMenu(groupName.c_str()))
            continue;
        // 各項目
        for (std::map<std::string, Item>::iterator itItem = group.begin();
            itItem != group.end(); ++itItem) {
            // 項目名を取得
            const std::string& itemName = itItem->first;
            // 項目の参照を取得
            Item& item = itItem->second;

            // int32_t
            if (std::holds_alternative<int32_t>(item)) {
                int32_t* ptr = std::get_if<int32_t>(&item);
                ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
            }
            // float
            if (std::holds_alternative<float>(item)) {
                float* ptr = std::get_if<float>(&item);
                ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);
            }
            // Vector3
            if (std::holds_alternative<Vector3>(item)) {
                Vector3* ptr = std::get_if<Vector3>(&item);
                ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
            }
        }

        ImGui::Text("\n");
        // 調整項目をセーブ
        if (ImGui::Button("Export")) {
            SaveFile(groupName);
            std::string message = std::format("{}.json saved.", groupName);
            MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
        }

        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
    ImGui::End();
}

void GlobalVariables::CreateGroup(const std::string& groupName)
{
    datas_[groupName];
}