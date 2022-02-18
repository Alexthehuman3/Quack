#pragma once
#include <GenericComponent.h>
#include <Vector3.h>
#include <Transform.h>
enum ComponentID
{
	GENERIC,
	TRANSFORM,
};
class GameObject
{
public:
	GameObject();
	GameObject(std::string _name);
	GameObject(Vector3 _pos);
	GameObject(Vector3 _pos, Vector3 _scale);
	GameObject(Vector3 _pos, Vector3 _scale, std::string _name);
	GameObject(Vector3 _pos, Vector3 _scale, Vector3 _rot);
	GameObject(Vector3 _pos, Vector3 _scale, Vector3 _rot, std::string _name);
	~GameObject() = default;

//	std::shared_ptr<GenericComponent>& getGeneric();
	std::shared_ptr<Transform>& getTransform();
	
	
	void Name(std::string new_name);
	std::string getName();
private:
	bool enabled = true;
	std::vector<std::shared_ptr<GenericComponent>> components;
	std::shared_ptr<Transform> transform = nullptr;
	std::string name = "Default";
};

