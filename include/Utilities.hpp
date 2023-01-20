namespace Core
{	
	float toRadians = 0.0174532925;
	
	//smooth functions
	float lerp(float a, float b, float t)
	{
		return a * (1.0 - t) + (b * t);
	}
	float smoothstep(float a, float b, float t)
	{
		float t1 = t*t;
		float t2 = 1 - (1-t)*(1-t);
		return lerp(a, b, lerp(t1, t2, t));
	}

	//round
	int myRound(float n)
	{
		return floor(n+0.5);
	}
	float myRound(float n, int u)
	{
		return floor((n+0.5)*(10*u))/(10*u);
	}
	float myRound(float n, int u, bool i)
	{
		if (i == false)
			return floor(n*(10*u))/(10*u);
		return floor((n+0.5)*(10*u))/(10*u);
	}
	
	int randomRange(int min, int max) //range : [min, max]
	{
	   return min + rand() % (( max + 1 ) - min);
	}
	
	template<typename T>
	T clampInt(T num, float min, float max)
	{
		if (num > max)
			return static_cast<T>(max);
		else if (num < min)
			return static_cast<T>(min);
		
		return num;
	}
	//now do not need
	int** bilinearResize2DArray(int sX, int sY, int** arrayFrom, int sizeOfArrayFromX, int sizeOfArrayFromY)
	{
		float ratioX = static_cast<float>(sizeOfArrayFromY-1)/static_cast<float>(sX-1);
		float ratioY = static_cast<float>(sizeOfArrayFromX-1)/static_cast<float>(sY-1);
		
		int** arrayTo = new int*[sY];
		for (int h = 0; h < sY; h++)
			arrayTo[h] = new int[sX];
		
		for (int y = 0; y < sY; y++)
		{
			for (int x = 0; x < sX; x++)
			{
				float tempX = x*ratioX;
				float tempY = y*ratioY;
				
				float weightX = tempX-floor(tempX);
				float weightY = tempY-floor(tempY);

				int x1y1 = arrayFrom[static_cast<int>(floor(tempY))][static_cast<int>(floor(tempX))];
				int x2y1 = arrayFrom[static_cast<int>(floor(tempY))][static_cast<int>(ceil(tempX))];
				int x1y2 = arrayFrom[static_cast<int>(ceil(tempY))][static_cast<int>(floor(tempX))];
				int x2y2 = arrayFrom[static_cast<int>(ceil(tempY))][static_cast<int>(ceil(tempX))];
				
				arrayTo[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
			}
		}
		return arrayTo;
	}
	
	struct vec2
	{
		float x;
		float y;
		
		vec2()
		{
			x = 0;
			y = 0;
		}
		vec2(float nxy)
		{
			x = nxy;
			y = nxy;
		}
		vec2(float nx, float ny)
		{
			x = nx;
			y = ny;
		}
		
		vec2 operator + (vec2 const &secondVec) const
		{
			return vec2(x + secondVec.x, y + secondVec.y);
		}
		vec2 operator - (vec2 const &secondVec) const
		{
			return vec2(x - secondVec.x, y - secondVec.y);
		}
		vec2 operator / (vec2 const &secondVec) const
		{
			return vec2(x / secondVec.x, y / secondVec.y);
		}
		vec2 operator * (vec2 const &secondVec) const
		{
			return vec2(x * secondVec.x, y * secondVec.y);
		}
	};
	
	vec2 vec2Lerp(vec2 num1, vec2 num2, float t)
	{
		vec2 result = {lerp(num1.x, num2.x, t), lerp(num1.y, num2.y, t)};
		return result;
	}
	
	struct vec2Int
	{
		int x;
		int y;
		
		vec2Int()
		{
			x = 0;
			y = 0;
		}
		vec2Int(int nxy)
		{
			x = nxy;
			y = nxy;
		}
		vec2Int(int nx, int ny)
		{
			x = nx;
			y = ny;
		}
		
		vec2Int operator + (vec2Int const &secondVec) const
		{
			return vec2Int(x + secondVec.x, y + secondVec.y);
		}
		vec2Int operator - (vec2Int const &secondVec) const
		{
			return vec2Int(x - secondVec.x, y - secondVec.y);
		}
		vec2Int operator / (vec2Int const &secondVec) const
		{
			return vec2Int(x / secondVec.x, y / secondVec.y);
		}
		vec2Int operator * (vec2Int const &secondVec) const
		{
			return vec2Int(x * secondVec.x, y * secondVec.y);
		}
		vec2Int operator = (vec2 const &secondVec) const
		{
			return vec2Int(myRound(secondVec.x), myRound(secondVec.y));
		}
	};
	
	bool YSortVec2Int(vec2Int num1, vec2Int num2)
	{
		return (num1.y < num2.y);
	}
	bool YSortVec2(vec2Int num1, vec2Int num2)
	{
		return (num1.y < num2.y);
	}
	vec2Int myRound(vec2 n)
	{
		return vec2Int(floor(n.x+0.5), floor(n.y+0.5));
	}
	
	struct Color
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;
		Color (uint8_t myR, uint8_t myG, uint8_t myB, uint8_t myA)
		{
			r = myR;
			g = myG;
			b = myB;
			a = myA;
		}
		Color(bool random)
		{
			if (random)
			{
				r = randomRange(10, 254);
				g = randomRange(10, 254);
				b = randomRange(10, 254);
				a = 255;
			}
		}
		Color()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 255;
		}
	};
	
	Color colorLerp(Color num1, Color num2, float t)
	{
		Color result = {static_cast<uint8_t>(myRound(lerp(num1.r, num2.r, t))),
						static_cast<uint8_t>(myRound(lerp(num1.g, num2.g, t))),
						static_cast<uint8_t>(myRound(lerp(num1.b, num2.b, t))),
						static_cast<uint8_t>(myRound(lerp(num1.a, num2.a, t)))};
		return result;
	}

	SDL_Point getTextureSize(SDL_Texture *texture) 
	{
		SDL_Point size;
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		return size;
	}
}