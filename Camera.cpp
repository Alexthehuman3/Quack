#include "Camera.h"

Camera::Camera()
{
	Vector3 pos = { DEFAULT_X,DEFAULT_Y,DEFAULT_Z };
	Vector3 rot = { 0,0,0 };
	Vector3 scale = { 1,1,1 };
	transform = std::make_shared<Transform>(pos,rot,scale);
}

void Camera::init(ID3D11Device* device, int _width, int _height)
{
	width = _width;
	height = _height;
	initCamera(width,height);
}

void Camera::Free()
{
	
}

void Camera::Update(double deltaTime)
{
	
	if (moveable)
	{

		//XMVECTOR deltaTimeMove = XMVectorSet( vertical * deltaTime, 1.0f, vertical * deltaTime, 0.0f );
		//Variables for Camera Matrix - Sets position, Target Direction, and sets the Up of the camera.
		camPosition = XMVectorSet(transform->getXPos(), transform->getYPos(), transform->getZPos(), 0.0f);
		camRotation = XMMatrixRotationRollPitchYaw(0,yaw,0);
		camTarget = XMVector3TransformCoord(default_forward, camRotation);
		camTarget = XMVector3Normalize(camTarget);

		XMMATRIX RotateYTemp;
		RotateYTemp = XMMatrixRotationY(yaw);

		camRight = XMVector3TransformCoord(default_right, RotateYTemp);
		camUp = XMVector3TransformCoord(camUp, RotateYTemp);
		camForward = XMVector3TransformCoord(default_forward, RotateYTemp);

		moveBackForward = (moveBackForward + vertical) * (deltaTime/200);

		//camForward = camForward * deltaTimeMove;
		camPosition += moveBackForward * camForward;
		transform->Position(XMVectorGetByIndex(camPosition, 0), XMVectorGetByIndex(camPosition, 1), XMVectorGetByIndex(camPosition, 2));

		moveBackForward = 0.0f;

		camTarget = camPosition + camTarget;
		//Sets the variable into the matrix.
		camView = std::make_shared<XMMATRIX>(XMMatrixLookAtLH(camPosition, camTarget, camUp));
		yaw += horizontal * deltaTime / 400;
	}
	
}

void Camera::Draw(ID3D11DeviceContext* devCon)
{
	/*
	//Creates WVP Matrix sent to Vertex Shader to reposition object's vertices.
	//Should be done for each object in the scene.
	World = XMMatrixIdentity();

	//Defines WVP by multiplying what we've done above
	WVP = World * camView * camProjection;

	//Updates Constant Buffer
	cbPerObj.WVP = XMMatrixTranspose(WVP);

	devCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	*/
}

void Camera::initCamera(int width, int height)
{
	//Variables for Camera Matrix - Sets position, Target Direction, and sets the Up of the camera.
	camForward = XMVectorSet(0, 0, 1, 0);               //sets Forward
	camPosition = XMVectorSet(transform->getXPos(), transform->getYPos(), transform->getZPos(), 0.0f);  //Sets Intial Camera Position 
	camTarget = XMVectorSet(transform->getXPos(), transform->getYPos(), transform->getZPos(), 0.0f);    //Target facing location based on transform's position
	camTarget += camForward;																   //Adds Forward to make the next movement step a forward in the right direction.
	camUp = XMVectorSet(transform->getXPos(), transform->getYPos() + 1, transform->getZPos(), 0.0f);		// The up of the camera required for an XMMATRIX
	camDown = XMVectorSet(transform->getXPos(), transform->getYPos() - 1, transform->getZPos(), 0.0f);
	camRight = XMVectorSet(transform->getXPos()+1, transform->getYPos(), transform->getZPos(), 0.0f);

	//Sets the variable into the matrix.
	camView = std::make_shared<XMMATRIX>(XMMatrixLookAtLH(camPosition, camTarget, camUp));

	/*
	* Projection Space(the cone with a chopped pyramid top, you can visualize this very easily with Unity)
	* FOV - Field of View in Radian along y-axis
	* Aspect Ratio - Width divided by height in the smallest number (4/3,16/9,16/10 etc)
	* Near Z - Constant Float in Header, distance from Camera to Near Z Plane
	* Far Z - Constant Float in Header, distance from Camera to Farplane;
	*/
	camProjection = std::make_shared<XMMATRIX>(XMMatrixPerspectiveFovLH(FOV, (float)width/height, NEAR_Z,FAR_Z));
}



std::shared_ptr<XMMATRIX> Camera::CamView()
{
	return camView;
}

std::shared_ptr<XMMATRIX> Camera::CamProjection()
{
	return camProjection;
}

std::shared_ptr<Transform>& Camera::getTF()
{
	return transform;
}
