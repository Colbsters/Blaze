#include "Application.h"

int main()
{
	auto* app = CreateApplication();

	app->OnCreate();

	while (app->OnUpdate());

	app->OnDestroy();

	delete app;

	return 0;
}
