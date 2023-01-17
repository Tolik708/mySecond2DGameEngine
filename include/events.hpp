namespace Input
{
	void events()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
		}
	}
	
	vec2 mousePos()
	{
		int x;
		int y;
		SDL_GetMouseState(&x, &y);
		vec2 result = {static_cast<float>(x), static_cast<float>(y)};
		return result;
	}
}