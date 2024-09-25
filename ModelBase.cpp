#include "ModelBase.h"

ModelBase* ModelBase::instance_ = nullptr;

ModelBase* ModelBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new ModelBase;
	}
	return instance_;
}

void ModelBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateObject3dRootSignature().Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateObject3dPipelineState(false).Get();
}

void ModelBase::DrawBase()
{
	dxEngine_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	dxEngine_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	dxEngine_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}
