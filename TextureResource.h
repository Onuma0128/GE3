#pragma once
#include "LoadTexture.h"
#include "DescriptorHeap.h"
#include "DepthStencilTexture.h"
#include "MT3.h"
#include <array>

class TextureResource
{
public:
	~TextureResource();

	void Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, const uint32_t& descriptorSizeSRV);

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU(const std::string& filePath,const uint32_t& index);

private:
	ComPtr<ID3D12Device> device_ = {};
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_{};
	uint32_t descriptorSizeSRV_{};

	std::array<ComPtr<ID3D12Resource>, 10> textureResource_ = {};

	///=============================================================================================================

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};

	///=============================================================================================================

	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_{};

	///=============================================================================================================
};

