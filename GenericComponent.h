#pragma once
#include <memory>
#include <vector>
#include <string>
class GenericComponent
{
public:
	GenericComponent() = default;
	~GenericComponent() = default;

	std::string Name();

private:
	bool enabled = true;
	std::string name = "Default";
};

