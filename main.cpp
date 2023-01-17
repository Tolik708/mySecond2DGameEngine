// includes
#include "include/header.hpp"

namespace Core
{
	//esseential variables
	SDL_Window* win;
	SDL_Renderer* rend;
	bool running = true;
	float FPS = 60;
}

using namespace Core;
#include "include/Utilities.hpp"
#include "include/events.hpp"

namespace Core
{
	void UpdateCore()
	{
		//draw
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		//actuale code here
		SDL_RenderPresent(rend);
	}
	
	void InitialzeAll()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		win = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
		rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	}
	
	void Quit()
	{
		SDL_DestroyWindow(win);
		SDL_Quit();
	}
}

int main(int argc, char *args[])
{
	InitialzeAll();
	
	//FPS limitation
	int a = 1;
	int b = 1;
	float delta = 0;
	
	//main loop
	while (running)
	{
		a = SDL_GetTicks();
		delta = a - b;
		if (delta > 1000/FPS)
		{
			UpdateCore();
			std::cout << "all good" << std::endl;
			b = a;
		}
	}
	Quit();
	return 0;
}