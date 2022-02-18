#include "game.h"

Game::~Game()
{
	
//	delete doomTime;
//	doomTime = nullptr;
}

bool Game::Init(std::shared_ptr<PGRP> _pgrp, ID3D11DeviceContext* devCon)
{
	pgrp = _pgrp;
	
	InitiateLevel(devCon);
	return true;
}

void Game::Update(double deltaTime)
{
	/*
	rotY += rot_speed*deltaTime;
	if (rotY > 360.0f)
	{
		rotY = 0;
	}
	objs[3]->getTransform()->yRot(rotY);
	*/
	//SwayCamera();
	
	for (auto& enemy : enemies)
	{
		//enemy->getTransform()->LookAt(pgrp->Cam()->getTF());
	}
}

void Game::Render(ID3D11DeviceContext* devCon)
{
	/*
	for (auto& obj : objs)
	{
		pgrp->renderCube(devCon, obj);
	}
	*/
	for (auto& lvl1 : level1)
	{
		pgrp->renderCube(devCon, lvl1);
	}
	for (auto& enemy : enemies)
	{
		pgrp->renderCube(devCon, enemy);
	}
}

void Game::SwayCamera()
{
	pgrp->Cam()->getTF()->xPos(sway_horizontal);
	pgrp->Cam()->getTF()->yPos(sway_vertical);
	if (pgrp->Cam()->getTF()->getXPos() > 1 || pgrp->Cam()->getTF()->getXPos() < -1)
	{
		sway_horizontal *= -1;
	}
	if (pgrp->Cam()->getTF()->getYPos() > 1 || pgrp->Cam()->getTF()->getYPos() < -1)
	{
		sway_vertical *= -1;
	}
}

void Game::InitiateLevel(ID3D11DeviceContext* devCon)
{
	newObject();
	for (auto& obj : objs)
	{
		pgrp->makeCube(devCon, obj);
	}

	for (auto& lvl1 : level1)
	{
		pgrp->makeCube(devCon, lvl1);
	}
}

void Game::KeyHandler()
{

}

void Game::newObject()
{
	objs.push_back(std::make_shared<GameObject>(Vector3{ 0,0,0 }, Vector3{ 20,1,20 }, "ground_plane"));
	objs.push_back(std::make_shared<GameObject>(Vector3{ -20,1,0 }, Vector3{ 1,10,20 }, "Left Wall"));
	objs.push_back(std::make_shared<GameObject>(Vector3{ 20,1,0 }, Vector3{ 1,10,20 }, "Right Wall"));
	objs.push_back(std::make_shared<GameObject>(Vector3{ 0,2,0 }, Vector3{ 1,1,1 }, "Spinny Cube")); //3
	constructLvl1();
}

void Game::constructLvl1()
{
	std::string blocks;

	std::ifstream LevelData("Level1.txt");
	float x = 0;
	float y = 0;
	float z = 0;
	while (getline(LevelData, blocks))
	{
		for (char& block : blocks)
		{
			if (block == 'w')
			{
				level1.push_back(std::make_shared<GameObject>(Vector3{ x,y + 2,z }, Vector3{ 1,1,1 }, "block"));
			}
			if (block == '-')
			{
				level1.push_back(std::make_shared<GameObject>(Vector3{ x,y,z }, Vector3{ 1,1,1 }, "block"));
			}
			if (block == 's')
			{
				level1.push_back(std::make_shared<GameObject>(Vector3{ x,y,z }, Vector3{ 1,1,1 }, "block"));
				pgrp->Cam()->getTF()->Position(x, y+2, z);
			}
			if (block == 'e')
			{
				level1.push_back(std::make_shared<GameObject>(Vector3{ x,y,z }, Vector3{ 1,1,1 }, "block"));
				enemies.push_back(std::make_shared<GameObject>(Vector3{ x,y-0.25f + 2,z }, Vector3{ 0.5,0.75,0.2 }, "enemy"));
			}
			x += 2;
		}
		z += 2;
		x = 0;
	}
}



