//***************************************************************************************
//LU SHUAI QIAO 2020
//Frame reference by Introduction to 3D Game Programming with Directx 12
//***************************************************************************************
#pragma once

#include "d3dUtil.h"
//立方体贴图索引
enum class CubeMapFace : int
{
	PositiveX = 0,
	NegativeX = 1,
	PositiveY = 2,
	NegativeY = 3,
	PositiveZ = 4,
	NegativeZ = 5
};

class CubeRenderTarget
{
public:
	CubeRenderTarget(ID3D12Device* device,
		UINT width, UINT height,
		DXGI_FORMAT format);
	//单例
	CubeRenderTarget(const CubeRenderTarget& rhs) = delete;
	CubeRenderTarget& operator=(const CubeRenderTarget& rhs) = delete;
	~CubeRenderTarget() = default;

    //d3d资源
	ID3D12Resource* Resource();

	CD3DX12_GPU_DESCRIPTOR_HANDLE Srv();//立方体贴图Srv
	CD3DX12_CPU_DESCRIPTOR_HANDLE Rtv(int faceIndex);//立方体贴图RTV
    //视口举行剪裁
	D3D12_VIEWPORT Viewport()const;
	D3D12_RECT ScissorRect()const;
	//创建描述符
	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuSrv,
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuRtv[6]);
	
	//更新大小
	void OnResize(UINT newWidth, UINT newHeight);

private:

	void BuildDescriptors();
	void BuildResource();//创建资源

private:
	//D3D驱动
	ID3D12Device* md3dDevice = nullptr;
	D3D12_VIEWPORT mViewport;
	D3D12_RECT mScissorRect;
	
	UINT mWidth = 0;
	UINT mHeight = 0;
	DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuSrv;//CPU SRV
	CD3DX12_GPU_DESCRIPTOR_HANDLE mhGpuSrv;//GPU SRV
	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuRtv[6]; //CPU RTV

	Microsoft::WRL::ComPtr<ID3D12Resource> mCubeMap = nullptr; //立方体贴图资源
};

