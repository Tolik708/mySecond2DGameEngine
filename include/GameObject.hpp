namespace Core
{
	struct GameObject
	{
		//general
		bool active = true;
		
		//transform
		vec2 position = vec2();
		vec2 size = vec2();
		float rotation = 0;
		
		//mesh
		mesh *myMesh;
		bool toDraw = true;
		
		void draw()
		{
			if (myMesh->texture != nullptr)
			{
				SDL_Rect pos;
				pos.x = position.x - (size.x/2);
				pos.y = position.y - (size.y/2);
				pos.w = size.x;
				pos.h = size.y;
				SDL_RenderCopyEx(rend, myMesh->texture, nullptr, &pos, rotation, NULL, SDL_FLIP_NONE);
			}
		}
		
		void update()
		{
			if (active)
			{
				if (toDraw && myMesh != nullptr)
					draw();
			}
		}
		
		//constructs
	};
	
	std::vector<GameObject> objects;
	
	GameObject AddObj(vec2 pos, mesh *newMesh)
	{
		GameObject newObj;
		newObj.myMesh = newMesh;
		newObj.position = pos;
		objects.push_back(newObj);
		return newObj;
	}
}