#include "PGRP.h"

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float cr, float cg, float cb, float ca) :
		pos(x, y, z), color{ cr, cg, cb, ca } {}
	Vector3 pos;
	RGBA color;
};
struct TexVertex
{
	TexVertex() {}
	TexVertex(float x, float y, float z, 
			  float u, float v,
			  float nx, float ny, float nz) : pos(x, y, z), texCoord(u, v), normal(nx,ny,nz) {}
	Vector3 pos;
	Vector2 texCoord;
	Vector3 normal;
};

D3D11_INPUT_ELEMENT_DESC layout[]
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA,0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
UINT numElements = ARRAYSIZE(layout);

D3D11_INPUT_ELEMENT_DESC TexLayout[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
UINT texNumElements = ARRAYSIZE(TexLayout);

PGRP::PGRP()
{
	cam = std::make_shared<Camera>();
}


void PGRP::Free()
{
	squareVertBuffer->Release();
	depthStencilBuffer->Release();
	depthStencilView->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
	squareIndexBuffer->Release();
	cbPerObjectBuffer->Release();
	cam->Free();
	cbPerFrameBuffer->Release();
	
}

void PGRP::Update(double deltaTime)
{
	rotY += cycleSpeed * deltaTime/1000;
	if (rotY > 1 || rotY < -1)
	{
		cycleSpeed *= -1;
	}
	light.dir = Vector3{ rotY/2 ,rotY, rotY/4 };
	cam->Update(deltaTime);
}

void PGRP::Draw(ID3D11DeviceContext* devCon)
{
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	constbuffPerFrame.light = light;
	devCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
	devCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);
	devCon->VSSetShader(VS, 0, 0);
	devCon->PSSetShader(PS, 0, 0);
	cam->Draw(devCon);
//	ReDrawAllObjects(devCon);
}

/*
void PGRP::ReDrawAllObjects(ID3D11DeviceContext* devCon)
{
	for (int i = 0; i < cubesWorld.size(); i++)
	{
		WVP = *cubesWorld[i].second * *cam->CamView() * *cam->CamProjection();
		cbPerObj.WVP = XMMatrixTranspose(WVP);
		devCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
		devCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
		devCon->DrawIndexed(36, 0, 0);
	}
}
*/

void PGRP::init(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, int width, int height, ID3D11RenderTargetView* _renderTargetView)
{
	initShaders(d3d11Device, d3d11DevCon);

	initTextures(d3d11Device, d3d11DevCon);

//	initIndices(d3d11Device, d3d11DevCon);

	initCubeIndices(d3d11Device, d3d11DevCon);

	initDepthStencilBuffer(d3d11Device, d3d11DevCon, width, height, _renderTargetView);

//	initColouredVertex(d3d11Device, d3d11DevCon);

	initTexturedVertex(d3d11Device, d3d11DevCon);
	
	createViewport(d3d11Device, d3d11DevCon, width, height);

	createConstBuffer(d3d11Device);

	cam->init(d3d11Device, width, height);

	//init all objects added by game
}

void PGRP::initShaders(ID3D11Device* device, ID3D11DeviceContext* devCon)
{
	HRESULT hr;

	hr = D3DCompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, &VS_Buffer, 0);
	hr = D3DCompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, &PS_Buffer, 0);

	hr = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	devCon->VSSetShader(VS, 0, 0);
	devCon->PSSetShader(PS, 0, 0);
	light.dir = Vector3{ 0.0f,1.0f, 0.0f };
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void PGRP::initTextures(ID3D11Device* device, ID3D11DeviceContext* devCon)
{
	HRESULT hr;
	hr = CreateWICTextureFromFile(device, L"seamless_dirt_ground_texture_by_hhh316_d4fon0w-pre.jpg", NULL, &CubesTexture);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
}

void PGRP::initIndices(ID3D11Device* device, ID3D11DeviceContext* devCon)
{
	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);
	devCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void PGRP::initCubeIndices(ID3D11Device* device, ID3D11DeviceContext* devCon)
{
	DWORD TexIndices[] = {
		// Front Face
        0,  1,  2,
        0,  2,  3,
    
        // Back Face
        4,  5,  6,
        4,  6,  7,
    
        // Top Face
        8,  9, 10,
        8, 10, 11,
    
        // Bottom Face
        12, 13, 14,
        12, 14, 15,
    
        // Left Face
        16, 17, 18,
        16, 18, 19,
    
        // Right Face
        20, 21, 22,
        20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = TexIndices;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);
	devCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void PGRP::initDepthStencilBuffer(ID3D11Device* device, ID3D11DeviceContext* devCon, int width, int height, ID3D11RenderTargetView* _renderTargetView)
{
	D3D11_TEXTURE2D_DESC desc;

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0; //max of 2046, the higher, the lower the FPS
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	device->CreateTexture2D(&desc, NULL, &depthStencilBuffer);
	device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	devCon->OMSetRenderTargets(1, &_renderTargetView, depthStencilView);
}

void PGRP::initColouredVertex(ID3D11Device* device, ID3D11DeviceContext* devCon)
{
	HRESULT hr;
	Vertex v[] =
	{
		Vertex(-1.0f, -1.0f, -1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(-1.0f, +1.0f, -1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(+1.0f, +1.0f, -1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(+1.0f, -1.0f, -1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(-1.0f, -1.0f, +1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(-1.0f, +1.0f, +1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(+1.0f, +1.0f, +1.0f, vertR, vertG, vertB, 1.0f),
		Vertex(+1.0f, -1.0f, +1.0f, vertR, vertG, vertB, 1.0f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

	//	D3D11_MAPPED_SUBRESOURCE ms;
	//	d3d11DevCon->Map(squareVertBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	//	memcpy(ms.pData, v, sizeof(v));
	//	d3d11DevCon->Unmap(squareVertBuffer, NULL);

		//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

	//Create the Input Layout
	hr = device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	devCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void PGRP::initTexturedVertex(ID3D11Device* device, ID3D11DeviceContext* devCon)
{
	HRESULT hr;
	TexVertex v[] =
	{
		// Front Face
		TexVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,0.0f, 0.0f, -1.0f),
		TexVertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f),
		TexVertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f),
		TexVertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),

		// Back Face
		TexVertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,0.0f, 0.0f, 1.0f),
		TexVertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		TexVertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		TexVertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,0.0f, 0.0f, 1.0f),

		// Top Face
		TexVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
		TexVertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,  0.0f),
		TexVertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f),
		TexVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),

		// Bottom Face
		TexVertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f),
		TexVertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f),
		TexVertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f),
		TexVertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,0.0f, -1.0f,  0.0f),

		// Left Face
		TexVertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, 0.0f,  0.0f),
		TexVertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 0.0f,  0.0f),
		TexVertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f),
		TexVertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f),

		// Right Face
		TexVertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f),
		TexVertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f),
		TexVertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f),
		TexVertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TexVertex) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(TexVertex);
	UINT offset = 0;
	devCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

	//Create the Input Layout
	hr = device->CreateInputLayout(TexLayout, texNumElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	devCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void PGRP::createViewport(ID3D11Device* device, ID3D11DeviceContext* devCon, int width, int height)
{
	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	devCon->RSSetViewports(1, &viewport);
}

void PGRP::createConstBuffer(ID3D11Device* device)
{
	//creates the constant buffer
	HRESULT hr;
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	//constructs buffer with the setting above
	hr = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	hr = device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);
}

void PGRP::makeCube(ID3D11DeviceContext* devCon, std::shared_ptr<GameObject> obj)
{
	//Creates WVP Matrix sent to Vertex Shader to reposition object's vertices.
	//Done for every object within the std pair, which has an identifiable name.

	XMMATRIX newCube;     //constructs a new copy of an XMMATRIX containing a cube
	float x = obj->getTransform()->getXPos();
	float y = obj->getTransform()->getYPos();
	float z = obj->getTransform()->getZPos();
	float rot_x = obj->getTransform()->getXRot();
	float rot_y = obj->getTransform()->getYRot();
	float rot_z = obj->getTransform()->getZRot();
	float scale_x = obj->getTransform()->getScale().x;
	float scale_y = obj->getTransform()->getScale().y;
	float scale_z = obj->getTransform()->getScale().z;

	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(rot_z, rot_x, rot_y);  //Rotates the object based on the Rotation Value as Roll,Pitch and Yaw
	XMMATRIX Translation = XMMatrixTranslation(x, y, z);					//Translates the object as offset from 0,0,0.
	XMMATRIX Scale = XMMatrixScaling(scale_x, scale_y, scale_z);			//Applies the scale of the object
	newCube = Translation * Rotation * Scale;								//Applies the Translation, Rotation and Scale into the new cube matrix

	//Defines WVP by multiplying what we've done above
	WVP = newCube * *cam->CamView() * *cam->CamProjection();/* Defines our Initial World View
															 * Our Worldview becomes filled with a matrix that contains a cube's position,rotation and scale,
															 * it is then multiplied by the camera's perspective which is its view and the projection, a pyramid
															 * cone with the tip cut off.
															 */

	//Updates Constant Buffer
	cbPerObj.World = XMMatrixTranspose(WVP);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devCon->PSSetShaderResources(0, 1, &CubesTexture);
	devCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	devCon->DrawIndexed(36, 0, 0); //Draws the indices

	auto cube_pair = std::make_pair<std::string, std::shared_ptr<XMMATRIX>>(obj->getName(), std::make_shared<XMMATRIX>(newCube));
	/**
	* Work in Progress auto Name Assigner using Numbers similar to how Unity Handles game objects with the same name
	for (auto& cube_name : cubesWorld)					//searches through every cube inside the Vector
	{
		if (obj->getName() == cube_name.first)          //checks if there are duplicate names
		{
			bool reached_underscore = false;
			std::string new_name;
			for (char x : cube_name.first)              //Loops through every single character
			{
				if (!reached_underscore)                //If we haven't reached underscore yet
				{             
					new_name.push_back(x);				//Keep adding new letters to the string list
				}
				else
				{
					std::string objectNumber;								
					int name_number = std::atoi(objectNumber.c_str());
					name_number++;
					new_name.push_back(name_number);
				}
				if (x == '_')                           //If it reaches an underscore, it seperates out the number that comes after it
				{
					reached_underscore = true;
				}
			}
		}
	}
	*/
	cubesWorld.push_back(cube_pair);
	/* Constructs a pair of value automatically
	 * std::make_pair<std::string,std::shard_ptr<XMMATRIX> - creates a pair of type std::string and a shared pointer of type XMMATRIX
	 * Content (stuff inside the brackets): A string, and constructs a shared pointer out of the newCube.
	 * 
	 * Cube is then pushed back into the cubesWorld vector, which is iterated through every single Draw and Update frame.
	 */
}

void PGRP::renderCube(ID3D11DeviceContext* devCon, std::shared_ptr<GameObject> obj)
{
	//Creates WVP Matrix sent to Vertex Shader to reposition object's vertices.
	//Done for every object within the std pair, which has an identifiable name.

	XMMATRIX newCube;     //constructs a new copy of an XMMATRIX containing a cube
	float x = obj->getTransform()->getPos().x;
	float y = obj->getTransform()->getPos().y;
	float z = obj->getTransform()->getPos().z;
	float rot_x = obj->getTransform()->getRot().x;
	float rot_y = obj->getTransform()->getRot().y;
	float rot_z = obj->getTransform()->getRot().z;
	float scale_x = obj->getTransform()->getScale().x;
	float scale_y = obj->getTransform()->getScale().y;
	float scale_z = obj->getTransform()->getScale().z;
	
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(rot_x, rot_y, rot_z);  //Rotates the object based on the Rotation Value as Roll,Pitch and Yaw
	XMMATRIX Translation = XMMatrixTranslation(x, y, z);					//Translates the object as offset from 0,0,0.
	XMMATRIX Scale = XMMatrixScaling(scale_x, scale_y, scale_z);			//Applies the scale of the object
	newCube = Scale * Rotation * Translation;								//Applies the Translation, Rotation and Scale into the new cube matrix

	//Defines WVP by multiplying what we've done above
	WVP = newCube * *cam->CamView() * *cam->CamProjection();/* Defines our Initial World View
															 * Our Worldview becomes filled with a matrix that contains a cube's position,rotation and scale,
															 * it is then multiplied by the camera's perspective which is its view and the projection, a pyramid
															 * cone with the tip cut off.
															 */

	cbPerObj.World = XMMatrixTranspose(newCube);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	devCon->PSSetShaderResources(0, 1, &CubesTexture);
	devCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	devCon->DrawIndexed(36, 0, 0); //Draws the indices
}

void PGRP::setCubeRotation(std::string _name, std::shared_ptr<GameObject> obj)
{
	for (auto cube : cubesWorld)
	{
		if (cube.first == _name)
		{
			std::shared_ptr<XMMATRIX> _cube = cube.second;
			
		}
	}
}

std::shared_ptr<Camera> PGRP::Cam()
{
	return this->cam;
}


