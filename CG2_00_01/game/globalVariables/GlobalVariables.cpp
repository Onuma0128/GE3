#include "GlobalVariables.h"

#include "imgui.h"

GlobalVariables* GlobalVariables::instance_ = nullptr;

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
        for (std::map<std::string, Item>::iterator itItem = group.items.begin();
            itItem != group.items.end(); ++itItem) {
            // 項目名を取得
            const std::string& itemName = itItem->first;
            // 項目の参照を取得
            Item& item = itItem->second;

            // int32_t
            if (std::holds_alternative<int32_t>(item.value)) {
                int32_t* ptr = std::get_if<int32_t>(&item.value);
                ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
            }
            // float
            if (std::holds_alternative<float>(item.value)) {
                float* ptr = std::get_if<float>(&item.value);
                ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);
            }
            // Vector3
            if (std::holds_alternative<Vector3>(item.value)) {
                Vector3* ptr = std::get_if<Vector3>(&item.value);
                ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
            }
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