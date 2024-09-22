#pragma once
#include "wrl.h"
#include "DirectXEngine.h"

using Microsoft::WRL::ComPtr;

class SpriteBase
{
public:

	void Initialize(DirectXEngine* dxEngine);

	void DrawBase();


	// ゲッター
	DirectXEngine* GetDxEngine() const { return dxEngine_; }

private:

	DirectXEngine* dxEngine_ = nullptr;

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature_;
	// パイプラインステート
	ID3D12PipelineState* pipelineState_;

};