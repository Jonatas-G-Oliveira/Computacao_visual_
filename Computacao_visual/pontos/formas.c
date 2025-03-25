// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

// --------------------------------------------------------------------------------
// Constants and Enums
// --------------------------------------------------------------------------------

static const char *TITULO_JANELA = "Render Basic";

enum constantes{
	LARGURA = 640,
	ALTURA = 480,
	PONTOS = 128,
	COLOR_MAX = 255,
};
// --------------------------------------------------------------------------------
// Gloabals
// --------------------------------------------------------------------------------
static SDL_Window *janela = NULL;
static SDL_Renderer *renderizador = NULL;

// --------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------

static SDL_AppResult iniciar(void){
	SDL_Log("Inicializado o SDL...\n");
	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("*** Erro ao iniciar a SDL: %s\n",SDL_GetError());
		return SDL_APP_FAILURE;
	}	
	
	SDL_Log("Criando janela e renderizador...\n");
	if(!SDL_CreateWindowAndRenderer(TITULO_JANELA,LARGURA,ALTURA,0,&janela,&renderizador))
	{
		SDL_Log("Erro ao criar a janela e/ou renderizador: %s\n",SDL_GetError());
		return SDL_APP_FAILURE;
	}
	return SDL_APP_CONTINUE;
}

static void desligar(void){
	SDL_Log("Destruindo renderizador...\n");
	SDL_DestroyRenderer(renderizador);
	renderizador = NULL;
	
	SDL_Log("Destruindo janela...\n");
	SDL_DestroyWindow(janela);
	janela = NULL;
	SDL_Quit();
	
}


static void loop(){
	//Declaracao 
	srand(time(NULL));
	
	SDL_Color cor = { .r = 0, .g = 0, .b = 0, .a = COLOR_MAX };
	
	SDL_Rect retangulo = { .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f};
	
	//Definindo posicoes aleatorias
	SDL_FPoint pontos[PONTOS];
	for(size_t i = 0; i < PONTOS; i++){
		pontos[i].x = rand() % LARGURA;
		pontos[i].y = rand() % ALTURA;
	}
	
		
	SDL_FPoint cursorMouse = { .x = 0.0f, .y = 0.0f };
	SDL_HideCursor();
	
	
	//Looping d eexucação
	SDL_Event evento;
	char buffer[64] = { 0 };
	bool execucao = true;
	while(execucao)
	{
		while(SDL_PollEvent(&evento))
		{
			switch(evento.type)
			{
				case SDL_EVENT_QUIT:
				execucao = false;	
				break;
				
				case SDL_EVENT_MOUSE_MOTION:
				snprintf(buffer,64,"%s (%.0f %.0f)",TITULO_JANELA, evento.motion.x,evento.motion.y);
                SDL_SetWindowTitle(janela,buffer);
                break;
			}
			
		}
		SDL_SetRenderDrawColor(renderizador, 0, 0, 0, COLOR_MAX);
		SDL_RenderClear(renderizador);
		
		
		cor.r = rand() % COLOR_MAX;
		cor.g = rand() % COLOR_MAX;
		cor.b = rand() % COLOR_MAX;
		SDL_SetRenderDrawColor(renderizador, cor.r, cor.g, cor.b, cor.a);
		
		SDL_RenderPoints(renderizador,pontos,PONTOS);
		
		
		SDL_RenderPresent(renderizador);
		SDL_Delay(50);
		
	}
}

int main(int argc, char *argv[]){
	atexit(desligar);
	if(iniciar() == SDL_APP_FAILURE){
		return SDL_APP_FAILURE;
	}
	
	loop();
	

	return 0;
}
