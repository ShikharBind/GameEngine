#include <Scotch.h>


class Sandbox : public Scotch::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox() 
	{

	}
};

Scotch::Application* Scotch::CreateApplication()
{
	return new Sandbox();
}