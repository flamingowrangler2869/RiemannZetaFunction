#include <SDL2/SDl.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

#define WIDTH  800
#define HEIGHT 600
#define LIMIT  10000
#define SCALE  50

const double dt = 0.02;

// from https://en.wikipedia.org/wiki/Riemann_zeta_function#Riemann's_functional_equation
double complex zeta2(double complex s) {
	double complex sum = 0.0;
	for (int n = 1; n < LIMIT; n++)
		sum += pow(-1, n+1) / cpow(n, s);
	return (1.0/(1.0-cpow(2.0,1.0-s))) * sum;
}

int WinMain() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) printf("error initializing SDL2: %s\n", SDL_GetError());

	SDL_Window	 *window   = SDL_CreateWindow("Riemann Zeta Function", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event event;
	const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
	SDL_SetRenderDrawColor(renderer, 32,32,32,255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 242,242,242,255);
	double complex z0, z1;
	for (double t = 0 ;; t += dt) {
		if (SDL_PollEvent(&event), event.type == SDL_QUIT) goto exit;
		if (keyboard_state[SDL_SCANCODE_ESCAPE]) goto exit;

		// easy to optimize, but i'm leaving it like this for clarity
		z0 = zeta2(0.5 + -t*I)*SCALE + WIDTH/2 + HEIGHT/2*I;
		z1 = zeta2(0.5 + -(t+dt)*I)*SCALE + WIDTH/2 + HEIGHT/2*I;
		SDL_RenderDrawLine(renderer, creal(z0), cimag(z0), creal(z1), cimag(z1));
		SDL_RenderPresent(renderer);
	}
exit:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
