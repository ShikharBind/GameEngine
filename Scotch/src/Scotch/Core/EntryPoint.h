#pragma once

#ifdef SH_PLATFORM_WINDOWS

extern Scotch::Application* Scotch::CreateApplication();

int main(int argc, char** argv)
{
	Scotch::Log::Init();
	SH_CORE_WARN("Initialized Log!");

	auto app = Scotch::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif