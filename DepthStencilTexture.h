#pragma once
#include "wrl.h"
#include "ResourceObject.h"

using Microsoft::WRL::ComPtr;

ResourceObject CreateDepthStencilTextureResource(ComPtr<ID3D12Device> device, int width, int height);
