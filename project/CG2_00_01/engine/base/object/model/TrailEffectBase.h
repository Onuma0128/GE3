#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

using Microsoft::WRL::ComPtr;

class DirectXEngine;
class PipelineState;

class TrailEffectBase
{
private:

	static TrailEffectBase* instance_;

	TrailEffectBase() = default;
	~TrailEffectBase() = default;
	TrailEffectBase(TrailEffectBase&) = delete;
	TrailEffectBase& operator=(TrailEffectBase&) = delete;

public:

	static TrailEffectBase* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void DrawBase();

	void Finalize();

	/*==================== アクセッサ ====================*/

	DirectXEngine* GetDxEngine() const { return dxEngine_; }

private:

	DirectXEngine* dxEngine_ = nullptr;

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

};