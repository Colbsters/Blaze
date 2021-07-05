#include <iostream>

#include <Blaze/Blaze.h>

int main()
{
	Blaze::WindowCreateInfo wndCreateInfo =
	{
		Blaze::ObjectCreateInfo(),
		"Blaze Window",
		200, 100, 960, 540
	};
	auto window = Blaze::Window::Create(wndCreateInfo);

	std::cout << window->GetTitle() << '\n';

	while (window->IsRunning())
		window->Update();

	return 0;
}
