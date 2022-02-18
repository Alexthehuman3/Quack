#pragma once
struct SharedData
{
	//Windows Data
	const float width = 800;       //Width of the window
	const float height = 600;      //Height of the Window

	//Mouse
	bool left_pressed = false;     //Left Pressed or not Dictated by the core system
	bool right_pressed = false;    //Right Clicked or not dictated by core system
	double n_mouse_x = 0;          //Normalized Mouse X between -1 to 1
	double n_mouse_y = 0;          //Normalized Mouse Y between -1 to 1
	double mouse_x = 0;
	double mouse_y = 0;
};

