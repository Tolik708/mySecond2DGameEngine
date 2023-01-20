namespace Core
{
	enum meshType {non=0, line=1, box=2, circle=3, elipse=3, polygon=4};
	enum lineType {basic=0, bresenham=0, GuptaSproll=1};
	
	SDL_Surface* resizeImage(int nW, int nH, SDL_Surface* surfaceFrom)
	{
		//init 4 new arrays for 4 chanels
		uint8_t** newR = new uint8_t*[nH];
		uint8_t** newG = new uint8_t*[nH];
		uint8_t** newB = new uint8_t*[nH];
		uint8_t** newA = new uint8_t*[nH];
		for (int i = 0; i < nH; i++)
		{
			newR[i] = new uint8_t[nW];
			newG[i] = new uint8_t[nW];
			newB[i] = new uint8_t[nW];
			newA[i] = new uint8_t[nW];
		}
		
		uint8_t* pixelsFrom = (uint8_t*) surfaceFrom->pixels;
		
		//calculate ratio
		float ratioX = static_cast<float>(surfaceFrom->w-1)/static_cast<float>(nW-1);
		float ratioY = static_cast<float>(surfaceFrom->h-1)/static_cast<float>(nH-1);
		
		//calculate new colors
		for (int y = 0; y < nH; y++)
		{
			for (int x = 0; x < nW; x++)
			{
				float tempX = x*ratioX;
				float tempY = y*ratioY;
				
				float weightX = tempX-floor(tempX);
				float weightY = tempY-floor(tempY);
				
				int fx = static_cast<int>(floor(tempX)*surfaceFrom->pitch); //floor x
				int fy = static_cast<int>(floor(tempY)*surfaceFrom->format->BytesPerPixel); //floor y
				int cx = static_cast<int>(ceil(tempX)*surfaceFrom->pitch); //ceil x
				int cy = static_cast<int>(ceil(tempY)*surfaceFrom->format->BytesPerPixel); //ceil y
				
				uint8_t x1y1 = 0;
				uint8_t x2y1 = 0;
				uint8_t x1y2 = 0;
				uint8_t x2y2 = 0;
				
				//red
				x1y1 = pixelsFrom[fx + fy+2];
				x2y1 = pixelsFrom[cx + fy+2];
				x1y2 = pixelsFrom[fx + cy+2];
				x2y2 = pixelsFrom[cx + cy+2];
				newR[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
				
				//green
				x1y1 = pixelsFrom[fx + fy+1];
				x2y1 = pixelsFrom[cx + fy+1];
				x1y2 = pixelsFrom[fx + cy+1];
				x2y2 = pixelsFrom[cx + cy+1];
				newG[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
				
				//blue
				x1y1 = pixelsFrom[fx + fy];
				x2y1 = pixelsFrom[cx + fy];
				x1y2 = pixelsFrom[fx + cy];
				x2y2 = pixelsFrom[cx + cy];
				newB[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
				
				//alpha
				x1y1 = pixelsFrom[fx + fy+3];
				x2y1 = pixelsFrom[cx + fy+3];
				x1y2 = pixelsFrom[fx + cy+3];
				x2y2 = pixelsFrom[cx + cy+3];
				newA[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
			}
		}
		
		//creating new surface
		SDL_Surface* newSurface = SDL_CreateRGBSurface(0, nW, nH, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_LockSurface(newSurface);
		uint8_t* pixels = (uint8_t*)newSurface->pixels;
		
		//filling newSurface
		for (int y = 0; y < nH; y++)
		{
			for (int x = 0; x < nW; x++)
			{
				int tx = x*newSurface->pitch; //x
				int ty = y*newSurface->format->BytesPerPixel; //y
				
				pixels[tx+ty+2] = newR[y][x]; //r
				pixels[tx+ty+1] = newG[y][x]; //g
				pixels[tx+ty] = newB[y][x]; //b
				pixels[tx+ty+3] = newA[y][x]; //a
			}
		}
		
		SDL_UnlockSurface(newSurface);
		return newSurface;
	}
	
	struct mesh
	{
		meshType meshIs = non;
		Color meshColor;
		//width and height of new texture
		int width;
		int height;
		//saving current lowest point of vertesies for aligning to zeros
		int currLowestX;
		int currLowestY;
		
		SDL_Surface *tempSurface;
		SDL_Surface *textureSurface;
		SDL_Texture *texture;
		uint8_t *pixels;
		uint8_t *pixelsToLoad;
		
		char *pathToTexture;
		//saving pixels of mesh for quick moddifying
		uint8_t *pixelsOfMesh;
		int sizePixelsOfMesh;
		
		//box/circle
		vec2Int mSize;
		
		//line/polygon
		std::vector<vec2Int> vertesies;
		int vertexAmmount = 0;
		
		void generateMesh()
		{
			if (meshIs == non)
				return;
			
			createBounds();
			initTempSurface();
			
			std::cout << pathToTexture << std::endl;
			
			if (pathToTexture != nullptr)
				if (!initTextureSurface())
					return;
			
			switch(meshIs)
			{
				case line:
				{
					break;
				}
				case box:
				{
					for (int i = 0; i < height*tempSurface->pitch; i += 4)
					{
						pixelsOfMesh[i] = pixels[i] = meshColor.r;
						pixelsOfMesh[i+1] = pixels[i+1] = meshColor.g;
						pixelsOfMesh[i+2] = pixels[i+2] = meshColor.b;
						pixelsOfMesh[i+3] = pixels[i+3] = meshColor.a;
					}
					break;
				}
				case circle:
				{
					break;
				}
				case polygon:
				{
					break;
				}
				default:
				{
					return;
				}
			}
			createTexture();
		}
		
		//functions
		void createBounds()
		{
			switch(meshIs)
			{
				case line:
				{
					break;
				}
				case polygon:
				{
					//big numbers for init here for cases when lowest vertex is more then 0
					int lowestX = 9999;
					int lowestY = 9999;
					int hiegestX = -9999;
					int hiegestY = -9999;
					//comparing all vertesies and finding correct
					for (int i = 0; i < vertexAmmount; i++)
					{
						if (lowestX > vertesies[i].x)
							lowestX = vertesies[i].x;
						if (lowestY > vertesies[i].y)
							lowestY = vertesies[i].y;
						if (hiegestX < vertesies[i].x)
							hiegestX = vertesies[i].x;
						if (hiegestY < vertesies[i].y)
							hiegestY = vertesies[i].y;
					}
					//saving current lowest point of vertesies for alligening to zeros
					currLowestX = lowestX;
					currLowestY = lowestY;
					//make box a little bit bigger for sure
					lowestX--;
					lowestY--;
					hiegestX++;
					hiegestY++;
					//getting difference for knowing width and height
					height = hiegestY - lowestY;
					width = hiegestX - lowestX;
				}
				case circle:
				{
					width = mSize.x*2;
					height = mSize.y*2;
				}
				case box:
				{
					width = mSize.x;
					height = mSize.y;
				}
				default:
					return;
			}
		}
		
		void initTempSurface()
		{
			//creating surface rgba and storing it's pixels to pixels pointer
			tempSurface = SDL_CreateRGBSurface(0, height, width, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
			SDL_LockSurface(tempSurface);
			pixels = (uint8_t*)tempSurface->pixels;
			
			//init pixels of sprite array
			sizePixelsOfMesh = height*tempSurface->pitch;
			pixelsOfMesh = new uint8_t[sizePixelsOfMesh];
		}
		
		bool initTextureSurface()
		{
			//loading texture and checking if it needs to be resized
			SDL_Surface* tempImage = SDL_LoadBMP(pathToTexture);
			if (!tempImage)
			{
				std::cout << "something wrong with texture loading" << SDL_GetError() << std::endl;
				return false;
			}
			
			if (tempImage->w != width || tempImage->h != height)
				textureSurface = resizeImage(width, height, tempImage);
			else
				textureSurface = tempImage;
			
			SDL_FreeSurface(tempImage);
			
			pixelsToLoad = (uint8_t*) textureSurface->pixels;
			
			if (!textureSurface)
			{
				std::cout << "something wrong with texture loading" << SDL_GetError() << std::endl;
				return false;
			}
			
			return true;
		}
		
		void createTexture()
		{
			if (textureSurface != nullptr)
				SDL_FreeSurface(textureSurface);

			SDL_UnlockSurface(tempSurface);
			texture = SDL_CreateTextureFromSurface(rend, tempSurface);
			SDL_FreeSurface(tempSurface);
		}
		
		//constructors
		mesh (int type, vec2 *verts, int vertsSize, Color myCol)
		{
			meshIs = static_cast<meshType>(type);
			meshColor = myCol;
			
			vertesies.reserve(vertsSize);
			for (int i = 0; i < vertsSize; i++)
				vertesies.push_back(myRound(verts[i]));
			
			generateMesh();
		}
		
		mesh (int type, vec2 size, Color myCol)
		{
			meshIs = static_cast<meshType>(type);
			meshColor = myCol;
			
			mSize = size;
			
			generateMesh();
		}
		
		mesh (int type, vec2 *verts, int vertsSize, char *texturePath)
		{
			meshIs = static_cast<meshType>(type);
			pathToTexture = texturePath;
			
			vertesies.reserve(vertsSize);
			for (int i = 0; i < vertsSize; i++)
				vertesies.push_back(myRound(verts[i]));
			
			generateMesh();
		}
		
		mesh (int type, vec2 size, char *texturePath)
		{
			meshIs = static_cast<meshType>(type);
			pathToTexture = texturePath;
			
			mSize = size;
			
			generateMesh();
		}
	};
}