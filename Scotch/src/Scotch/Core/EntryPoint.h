#pragma once

#include "Scotch/Core/Application.h"
#include "Scotch/Core/Log.h"
#include "Scotch/Debug/Instrumentor.h"

#include <cstdlib>
#include <exception>
#include <memory>

#ifdef SH_PLATFORM_WINDOWS

extern Scotch::Application* Scotch::CreateApplication();

int main(int argc, char** argv)
{
	Scotch::Log::Init();

	try
	{
		SH_PROFILE_BEGIN_SESSION("Startup", "log/ScotchProfile-Startup.json");
		std::unique_ptr<Scotch::Application> app(Scotch::CreateApplication());
		SH_PROFILE_END_SESSION();

		SH_PROFILE_BEGIN_SESSION("Runtime", "log/ScotchProfile-Runtime.json");
		app->Run();
		SH_PROFILE_END_SESSION();

		SH_PROFILE_BEGIN_SESSION("Shutdown", "log/ScotchProfile-Shutdown.json");
		app.reset();
		SH_PROFILE_END_SESSION();
	}
	catch (const std::exception& error)
	{
		SH_PROFILE_END_SESSION();
		SH_CORE_ERROR("Application failed: {0}", error.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#endif
