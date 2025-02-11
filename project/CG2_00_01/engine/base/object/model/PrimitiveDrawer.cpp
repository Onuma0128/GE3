#include "PrimitiveDrawer.h"
#include "Camera.h"

PrimitiveDrawer* PrimitiveDrawer::instance_ = nullptr;

PrimitiveDrawer* PrimitiveDrawer::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new PrimitiveDrawer;
	}
	return instance_;
}

void PrimitiveDrawer::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateLine3dRootSignature();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateLine3dPipelineState();
}

void PrimitiveDrawer::DrawBase()
{
	dxEngine_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	dxEngine_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	dxEngine_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void PrimitiveDrawer::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

