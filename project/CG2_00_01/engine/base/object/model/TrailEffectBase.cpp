#include "TrailEffectBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

TrailEffectBase* TrailEffectBase::instance_ = nullptr;

TrailEffectBase* TrailEffectBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new TrailEffectBase;
	}
	return instance_;
}

void TrailEffectBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateTrailEffectRootSignature().Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateTrailEffectPipelineState().Get();
}

void TrailEffectBase::DrawBase()
{
	dxEngine_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	dxEngine_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	dxEngine_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void TrailEffectBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}