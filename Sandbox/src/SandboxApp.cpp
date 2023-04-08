#include <Scotch.h>

class ExampleLayer : public Scotch::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		SH_INFO("ExampleLayer::Update");
	}

	void OnEvent(Scotch::Event& event) override
	{
		SH_TRACE("{0}", event);
	}
};

class Sandbox : public Scotch::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{

	}
};

Scotch::Application* Scotch::CreateApplication()
{
	return new Sandbox();
}