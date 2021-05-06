#include <intercept.hpp>
#include <Windows.h>

static std::atomic<bool> g_running = false;
std::thread th;


DWORD __stdcall test_thread_main() {
	while (g_running.load()) {
		printf("Loop!\n");
		Sleep(500);
	}
	printf("Return..!\n");
	printf("exit..");
	return 0;
}


void start() {
	printf("Starting Thread..\n");
	g_running.store(true);
	th = std::thread(test_thread_main);
}

void stop() {
	g_running.store(false);
	printf("stop() - joining!\n");
	th.join();
	printf("stop() - join done!\n");
}


int intercept::api_version() { //This is required for the plugin to work.
    return INTERCEPT_SDK_API_VERSION;
}

void intercept::pre_start() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	start();
}

void intercept::handle_unload() {
	stop();
}