#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include "DirectXEngine.h"

using Microsoft::WRL::ComPtr;

class ModelBase
{
private:
	static ModelBase* instance_;

	ModelBase() = default;
	~ModelBase() = default;
	ModelBase(ModelBase&) = delete;
	ModelBase& operator=(ModelBase&) = delete;

public:
	// シングルトンインスタンスの取得
	static ModelBase* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void Finalize();


	// ゲッター
	DirectXEngine* GetDxEngine() const { return dxEngine_; }

private:
	DirectXEngine* dxEngine_ = nullptr;
};

