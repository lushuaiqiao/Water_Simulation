//***************************************************************************************
//LU SHUAI QIAO 2020
//Frame reference by Introduction to 3D Game Programming with Directx 12
//***************************************************************************************
#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

class Camera
{
public:

	Camera();
	~Camera();
	//���û�ȡλ��
	DirectX::XMVECTOR GetPosition()const;
	DirectX::XMFLOAT3 GetPosition3f()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);
	//��ȡ�����᷽��
	DirectX::XMVECTOR GetRight()const;
	DirectX::XMFLOAT3 GetRight3f()const;
	DirectX::XMVECTOR GetUp()const;
	DirectX::XMFLOAT3 GetUp3f()const;
	DirectX::XMVECTOR GetLook()const;
	DirectX::XMFLOAT3 GetLook3f()const;
	//��ȡԶ��ƽ�棬�����߱�
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;
	//��ȡ��Զƽ����
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;
	//������׶��
	void SetLens(float fovY, float aspect, float zn, float zf);
	//���ý���
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	//��ȡͶӰ�ӿھ���
	DirectX::XMMATRIX GetView()const;
	DirectX::XMMATRIX GetProj()const;

	DirectX::XMFLOAT4X4 GetView4x4f()const;
	DirectX::XMFLOAT4X4 GetProj4x4f()const;

	//�������
	void Strafe(float d);
	void Walk(float d);
	void Up(float d);

	void Pitch(float angle);
	void RotateY(float angle);

	//�����������
	void UpdateViewMatrix();

private:

	DirectX::XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mRight = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mUp = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 mLook = { 0.0f, 0.0f, 1.0f };

	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();
};

#endif // CAMERA_H