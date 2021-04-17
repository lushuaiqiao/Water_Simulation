#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount
	, UINT waterVertCount, UINT waterIndexCount)
{
	ThrowIfFailed(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CmdListAlloc.GetAddressOf())));

	PassCB = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
	MaterialBuffer = std::make_unique<UploadBuffer<MaterialData>>(device, materialCount, false);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);

	WaterVB = std::make_unique<UploadBuffer<Vertex>>(device, waterVertCount, false);
	WaterIB = std::make_unique<UploadBuffer<std::uint16_t>>(device, waterIndexCount, false);
}

FrameResource::~FrameResource()
{

}