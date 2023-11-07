#pragma once

#ifdef SH_PLATFORM_WINDOWS

extern Scotch::Application* Scotch::CreateApplication();

int main(int argc, char** argv)
{
	Scotch::Log::Init();

	SH_PROFILE_BEGIN_SESSION("Startup", "log/ScotchProfile-Startup.json");
	auto app = Scotch::CreateApplication();
	SH_PROFILE_END_SESSION();

	SH_PROFILE_BEGIN_SESSION("Runtime", "log/ScotchProfile-Runtime.json");
	app->Run();
	SH_PROFILE_END_SESSION();

	SH_PROFILE_BEGIN_SESSION("Shutdown", "log/ScotchProfile-Shutdown.json");
	delete app;
	SH_PROFILE_END_SESSION();

	return 0;
}

#endif