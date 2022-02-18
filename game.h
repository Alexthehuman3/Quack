#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <Vector3.h>
#include <memory>
#include <PGRP.h>
#include <GameObject.h>
#include <DoomTime.h>
#include <iostream>
#include <fstream>

class Game
{
public:
	Game() = default;
	~Game();

	bool Init(std::shared_ptr<PGRP> _pgrp, ID3D11DeviceContext* devCon);
	void Update(double deltaTime);
	void Render(ID3D11DeviceContext* devCon);
//	void bgColorR(int modr);
//	void bgColorG(int modg);
//	void bgColorB(int modb);
	void SwayCamera();

private:
	void InitiateLevel(ID3D11DeviceContext* devCon);
	void KeyHandler();
	void newObject();
	void constructLvl1();
	float sway_horizontal = 0.0003f;
	float sway_vertical = 0.0005f;
	float rot = 0;
	float rot_speed = 0.1;
	std::shared_ptr<PGRP> pgrp;
	std::vector<std::shared_ptr<GameObject>> objs;
	std::vector<std::shared_ptr<GameObject>> level1;
	std::vector<std::shared_ptr<GameObject>> enemies;
};

