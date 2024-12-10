#include "LightManager.h"

#include <numbers>
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

#include "CreateBufferResource.h"

LightManager* LightManager::instance_ = nullptr;

LightManager* LightManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new LightManager;
	}
	return instance_;
}

void LightManager::Initialize(DirectXEngine* dxEngine)
{
	this->dxEngine_ = dxEngine;

	MakeDirectionalLightData();

	MakePointLightData();

	MakeSpotLightData();
}

void LightManager::Update()
{
	directionalLightData_->direction = directionalLightData_->direction.Normalize();

	spotLightData_->direction = spotLightData_->direction.Normalize();
}

void LightManager::Debug_ImGui()
{
#ifdef _DEBUG
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx("Light", flag)) {
		if (ImGui::TreeNodeEx("Directional Light", flag)) {
			ImGui::ColorEdit4("LightColor", (float*)&directionalLightData_->color.x);
			ImGui::DragFloat3("DirectionalLightData.Direction", &directionalLightData_->direction.x, 0.01f);
			ImGui::DragFloat("DirectionalLightData.intensity", &directionalLightData_->intensity, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Point Light", flag)) {
			ImGui::ColorEdit4("LightColor", (float*)&pointLightData_->color.x);
			ImGui::DragFloat3("PointLightData.pos", &pointLightData_->position.x, 0.01f);
			ImGui::DragFloat("PointLightIntensity", &pointLightData_->intensity, 0.01f);
			ImGui::DragFloat("PointLightRadius", &pointLightData_->radius, 0.01f);
			ImGui::DragFloat("PointLightDecay", &pointLightData_->decay, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Spot Light", flag)) {
			ImGui::ColorEdit4("LightColor", (float*)&spotLightData_->color.x);
			ImGui::DragFloat3("SpotLightData.pos", &spotLightData_->position.x, 0.01f);
			ImGui::DragFloat3("SpotLightData.direction", &spotLightData_->direction.x, 0.01f);
			ImGui::DragFloat("SpotLightData.intensity", &spotLightData_->intensity, 0.01f);
			ImGui::DragFloat("SpotLightData.cosAngle", &spotLightData_->cosAngle, 0.01f);
			ImGui::DragFloat("SpotLightData.cosFalloffStart", &spotLightData_->cosFalloffStart, 0.01f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif // _DEBUG
}

void LightManager::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void LightManager::MakeDirectionalLightData()
{
	directionalLightResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(DirectionalLight)).Get();
	directionalLightBufferView_.BufferLocation = directionalLightResource_->GetGPUVirtualAddress();
	directionalLightBufferView_.SizeInBytes = sizeof(DirectionalLight);
	directionalLightBufferView_.StrideInBytes = sizeof(DirectionalLight);
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}

void LightManager::MakePointLightData()
{
	pointLightResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(PointLight)).Get();
	pointLightBufferView_.BufferLocation = pointLightResource_->GetGPUVirtualAddress();
	pointLightBufferView_.SizeInBytes = sizeof(PointLight);
	pointLightBufferView_.StrideInBytes = sizeof(PointLight);
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	// デフォルト値
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->position = { 0.0f,2.0f,2.0f };
	pointLightData_->intensity = 0.0f;
	pointLightData_->radius = 6.0f;
	pointLightData_->decay = 2.0f;
}

void LightManager::MakeSpotLightData()
{
	spotLightResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(SpotLight)).Get();
	spotLightBufferView_.BufferLocation = spotLightResource_->GetGPUVirtualAddress();
	spotLightBufferView_.SizeInBytes = sizeof(SpotLight);
	spotLightBufferView_.StrideInBytes = sizeof(SpotLight);
	spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	// デフォルト値
	spotLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightData_->position = { 2.0f,1.25f,0.0f };
	spotLightData_->distance = 7.0f;
	spotLightData_->direction = (Vector3{ -1.0f,-1.0f,0.0f }.Normalize());
	spotLightData_->intensity = 4.0f;
	spotLightData_->decay = 2.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 5.0f);
}
