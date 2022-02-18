#include "GameObject.h"

GameObject::GameObject()
{
    transform = std::make_shared<Transform>();
}
GameObject::GameObject(std::string _name)
{
    transform = std::make_shared<Transform>();
    name = _name;
}

GameObject::GameObject(Vector3 _pos)
{
    transform = std::make_shared<Transform>(_pos);
}

GameObject::GameObject(Vector3 _pos, Vector3 _scale)
{
    transform = std::make_shared<Transform>(_pos, _scale);
}

GameObject::GameObject(Vector3 _pos, Vector3 _scale, std::string _name)
{
    transform = std::make_shared<Transform>(_pos, _scale);
    name = _name;
}

GameObject::GameObject(Vector3 _pos, Vector3 _scale, Vector3 _rot)
{
    transform = std::make_shared<Transform>(_pos, _rot, _scale);
}

GameObject::GameObject(Vector3 _pos, Vector3 _scale, Vector3 _rot, std::string _name)
{
    transform = std::make_shared<Transform>(_pos, _rot, _scale);
    name = _name;
}

std::shared_ptr<Transform>& GameObject::getTransform()
{
    return transform;
}

void GameObject::Name(std::string new_name)
{
    name = new_name;
}

std::string GameObject::getName()
{
    return name;
}
