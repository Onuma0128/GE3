#include "Object3dBase.h"

Object3dBase* Object3dBase::instance_ = nullptr;

Object3dBase* Object3dBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Object3dBase;
	}
	return instance_;
}

void Object3dBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateObject3dRootSignature().Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateObject3dPipelineState().Get();
}

void Object3dBase::DrawBase()
{
	dxEngine_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	dxEngine_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	dxEngine_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3dBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}
