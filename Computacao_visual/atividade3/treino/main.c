#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>



//Para trabalharmos com imagens usando SDL,precisamos de uma janela e um rendereizador
//A funcao AteExit sera chamada sempre ao termino do programa,normalmente utilizadas para tarefas de limpeza
//Para carregar a imagem utilizamos DLS texteture,
//As dll's tem que estar na mesma pasta que o programa fonte

void shutdown(){
	SDL_Log("\n -> Fechando o SDL...");
	SDL_Quit();
}


int main(int argc, int *argv[]){	
	atexit(shutdown);

	//Inicializando o progama
	printf("\n -> Inicializando SDL ");
	if((SDL_Init(SDL_INIT_VIDEO)) == -1){
		printf("\n Nao foi possivel inicializar o programa %s ", SDL_GetError);
		return SDL_APP_FAILURE;
	}
	
	//Criando a janela e renderizador
	SDL_Window *janela;
	SDL_Renderer *renderizador;
	
	if(!SDL_CreateWindowAndRenderer("Titulo", 680, 480, 0, &janela, &renderizador)){
		printf("\n Nao foi possivel inicializar a janela %s", SDL_GetError);
		return SDL_APP_FAILURE;
	}
	
	//Carregando imagem					
	SDL_Texture *imagem = IMG_LoadTexture(renderizador, "assets/flor.jpg");
	if(!imagem){
		SDL_Log("Não foi possivel carregar a imagem %s",SDL_GetError);
	}
	
	
	SDL_FRect jpgRetangulo = {
		.x = 0,
		.y = 0,
		.w = imagem ? 680 : 0.0f,
		.h = imagem ? 480 : 0.0f,
	};
	
	
	//Loop
	char buffer[64] = { 0 };
	SDL_Event evento;
	int execucao = 1;
	while(execucao == 1){
		while(SDL_PollEvent(&evento)){//Enquanto a fila não tiver vazia
		
			switch(evento.type)
			{
				case SDL_EVENT_QUIT:
					execucao = -1;
					break;
				
				case SDL_EVENT_MOUSE_MOTION:
					snprintf(buffer,64,"%s (%.0f %.0f)","Janela", evento.motion.x, evento.motion.y);
					SDL_SetWindowTitle(janela, buffer);
					break;
			}
		}
		
		//Tem que passar o renderziador/imagem/O quadrado que vai ser utiizado
		SDL_SetRenderDrawColor(renderizador, 128, 128, 128, 255);
		SDL_RenderClear(renderizador);
		SDL_RenderTexture(renderizador,imagem,NULL,&jpgRetangulo);
		SDL_RenderPresent(renderizador);
		
	}
	
	SDL_DestroyWindow(janela);
	SDL_DestroyRenderer(renderizador);
	janela = NULL;
	return 0;
}
