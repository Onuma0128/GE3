#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include "DirectXEngine.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

class LightManager
{
private:
	static LightManager* instance_;

	LightManager() = default;
	~LightManager() = default;
	LightManager(LightManager&) = delete;
	LightManager& operator=(LightManager&) = delete;

public:

	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	struct PointLight {
		Vector4 color;
		Vector3 position;
		float intensity;
		float radius;
		float decay;
		float padding[2];
	};

	struct SpotLight {
		Vector4 color;
		Vector3 position;
		float intensity;
		Vector3 direction;
		float distance;
		float decay;
		float cosAngle;
		float cosFalloffStart;
		float padding;
	};

public:

	/*==================== メンバ関数 ====================*/

	// シングルトンインスタンスの取得
	static LightManager* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void Update();

	void Debug_ImGui();

	void Finalize();

	void MakeDirectionalLightData();

	void MakePointLightData();

	void MakeSpotLightData();

	/*==================== アクセッサー ====================*/

	ID3D12Resource* GetDirectionalLightResource()const { return directionalLightResource_.Get(); }

	ID3D12Resource* GetPointLightResource()const { return pointLightResource_.Get(); }

	ID3D12Resource* GetSpotLightResource()const { return spotLightResource_.Get(); }

	void SetPointLightPosition(const Vector3& position) { pointLightData_->position = position; }

private:

	DirectXEngine* dxEngine_ = nullptr;

	/*==================== 平行光源のデータ ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	D3D12_VERTEX_BUFFER_VIEW directionalLightBufferView_{};
	// バッファリソースの使い道を補足するバッファビュー
	DirectionalLight* directionalLightData_ = nullptr;

	/*==================== ポイントライトのデータ ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> pointLightResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	D3D12_VERTEX_BUFFER_VIEW pointLightBufferView_{};
	// バッファリソースの使い道を補足するバッファビュー
	PointLight* pointLightData_ = nullptr;

	/*==================== スポットライトのデータ ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> spotLightResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	D3D12_VERTEX_BUFFER_VIEW spotLightBufferView_{};
	// バッファリソースの使い道を補足するバッファビュー
	SpotLight* spotLightData_ = nullptr;

};