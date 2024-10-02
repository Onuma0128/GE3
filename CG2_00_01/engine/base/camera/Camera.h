#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include "DirectXEngine.h"

#include "Vector3.h"
#include "Transform.h"
#include "Matrix4x4.h"

using Microsoft::WRL::ComPtr;

class Camera
{
public:

	struct CameraForGPU {
		Vector3 worldPosition;
	};

private:

	static Camera* instance_;

	Camera() = default;
	~Camera() = default;
	Camera(Camera&) = delete;
	Camera& operator=(Camera&) = delete;

public:
	// シングルトンインスタンスの取得
	static Camera* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void Update();

	void Finalize();

	void MakeCameraData();

	/*==================== アクセッサ ====================*/

	// setter
	// RT部分
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }

	// カメラの設定
	void SetFovY(const float fovY) { fovY_ = fovY; }
	void SetAspectRatio(const float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(const float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(const float farClip) { farClip_ = farClip; }

	// getter
	const Matrix4x4& GetWorldMatrix()const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix()const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix()const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix()const { return viewProjectionMatrix_; }

	const Vector3& GetRotate()const { return transform_.rotate; }
	const Vector3& GetTranslate()const { return transform_.translate; }

	// カメラリソース
	ID3D12Resource* GetCameraResource()const { return cameraResource_.Get(); }

private:

	DirectXEngine* dxEngine_;

	/*==================== カメラの変数 ====================*/

	Transform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	Matrix4x4 projectionMatrix_;

	float fovY_;
	float aspectRatio_;
	float nearClip_;
	float farClip_;

	/*==================== カメラデータ ====================*/

	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraData_ = nullptr;
};

