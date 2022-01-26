#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window = NULL;
SDL_Renderer *renderer;
SDL_Texture* texture;

SDL_Texture *WID_loadTexture( SDL_Renderer *renderer, char *name);

uint64_t getTimeStamp() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		fprintf(stderr,"Failed to initialize SDL (%s)\n",SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("test SDL2",SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_SHOWN);

	if( window )
	{
		renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED); 

		if ( renderer )
		{
			SDL_RendererInfo info;
			SDL_GetRendererInfo(renderer, &info);
			fprintf(stderr, "name : %s\n", info.name);
			fprintf(stderr, "flags : ");
			if (info.flags & SDL_RENDERER_SOFTWARE) fprintf(stderr, "SDL_RENDERER_SOFTWARE");
			if (info.flags & SDL_RENDERER_ACCELERATED) fprintf(stderr, "SDL_RENDERER_ACCELERATED ");
			if (info.flags & SDL_RENDERER_PRESENTVSYNC) fprintf(stderr, "SDL_RENDERER_PRESENTVSYNC ");
			if (info.flags & SDL_RENDERER_TARGETTEXTURE) fprintf(stderr, "SDL_RENDERER_TARGETTEXTURE ");
			fprintf(stderr, "\nnb textures max : %d\n", info.num_texture_formats);
			fprintf(stderr, "texture size max : %d x %d \n", info.max_texture_width, info.max_texture_height);
			fprintf(stderr, "current driver : %s\n", SDL_GetCurrentVideoDriver());

			if (SDL_SetRenderTarget(renderer, NULL) != 0) {
				fprintf(stderr, "SDL_SetRenderTarget error %s\n", SDL_GetError());
			}
			
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer,0,0,0,255);
			SDL_RenderClear(renderer);

			texture = WID_loadTexture(renderer, (char*)"800_600.png");

			SDL_Rect clip = {0,0,200,200};

			int t1, t2;

			t1 = getTimeStamp();
			SDL_RenderCopy(renderer, texture, NULL, &clip);
			t2 = getTimeStamp();
			printf("RenderCopy 200x100 : %ld\n", (t2-t1));

			usleep(20000);
			
			t1 = getTimeStamp();
			SDL_RenderPresent(renderer);
			t2 = getTimeStamp();
			printf("RenderPresent : %ld\n", (t2 - t1));

			usleep(20000);
			
			t1 = getTimeStamp();
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			t2 = getTimeStamp();
			printf("RenderCopy 800x600 : %ld\n", (t2-t1));

			usleep(20000);
			
			t1 = getTimeStamp();
			SDL_RenderPresent(renderer);
			t2 = getTimeStamp();
			printf("RenderPresent : %ld\n", (t2 - t1));

			bool loop = true;
			while ( loop )
			{
				SDL_Event event;
				SDL_WaitEvent( &event );
				if ( event.type == SDL_QUIT )
				{
					fprintf(stderr, "received SDL_QUIT\n");
					loop = false;
				}
				if ( event.type == SDL_KEYDOWN )
				{
					if (event.key.keysym.sym == SDLK_q)
						loop = false;
				}
			}

            if (texture != NULL) SDL_DestroyTexture(texture);

		 SDL_DestroyRenderer(renderer);
		}
		else
		{
			fprintf(stderr,"Fail to create renderer (%s)\n",SDL_GetError());
		}
		SDL_DestroyWindow(window);
	}
	else
	{
		fprintf(stderr,"Fail to create window: %s\n",SDL_GetError());
	}

	SDL_Quit();

	return 0;
}


SDL_Texture *WID_loadTexture( SDL_Renderer *renderer, char *name)
{
	SDL_Texture *texture = NULL;

	if(name == NULL || renderer == NULL)
		return NULL;

	SDL_Surface* surface = IMG_Load( name );

	if (surface == NULL)
	{
		fprintf(stderr, "error loading : %s", name);
	}
	else
	{
		texture = SDL_CreateTextureFromSurface( renderer, surface );
		SDL_FreeSurface( surface );
	}

	return texture;
}
