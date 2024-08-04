#pragma once
#include "wrl.h"
#include "ResourceObject.h"

using Microsoft::WRL::ComPtr;

ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ComPtr<ID3D12Device> device, int width, int height);
