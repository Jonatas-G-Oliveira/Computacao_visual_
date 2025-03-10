// -----------------------------------------------------------------
// Autor: Jonatas Oliveira 
// Ano : 03/2025

// ----------------------------------------------
// Includes
// ----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// ---------------------------------------------
// Constantes 
// ---------------------------------------------
static const char TITULO[10] = "Janela";

enum Constantes {
	LARGURA = 780,
	ALTURA = 520
};

// -------------------------------------------
// Globals
// -------------------------------------------
static SDL_Window   *janela = NULL;
static SDL_Renderer *renderizador = NULL;
static SDL_Surface  *superficie = NULL;
static SDL_Texture  *textura = NULL;
static SDL_FRect retangulo = { .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 420};

// -------------------------------------------
// Declaracao de funcoes
// -------------------------------------------

/**
* SDL_Surface -> Software Rendering,armazenado na memoria RAM
* SDL_Texture -> Hardware Rendering,armazenado na VRAM e voce não tem acesso direto,porém renderiza muito mais rapido
*
*/
static void carregarRGBA32(char *caminhoDaImagem);


/**
* Vamos acessar cada pixel e modifcar a imagem
*
*/
static void inverterImagem(void);


static void shutdown(){
	SDL_Log("Destruindo textura..\n");
	SDL_DestroyTexture(textura);
	textura = NULL;
	
	SDL_Log("Destruindo superficie...\n");
	SDL_DestroySurface(superficie);
	superficie = NULL;
	
	SDL_Log("Destruindo renderizador...\n");
	SDL_DestroyRenderer(renderizador);
	renderizador = NULL;
	
	SDL_Log("Destruindo janela...\n");
	SDL_DestroyWindow(janela);
	janela = NULL;
	
	SDL_Log("\n -> Encerrando o SDL...");
	SDL_Quit();
}


static SDL_AppResult inicializar(){
	SDL_Log(" Inicializando SDL...\n");
	if((SDL_Init(SDL_INIT_VIDEO)) == -1){
		SDL_Log(" -> Erro ao iniciar a SDL: %s \n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	SDL_Log(" Criando a janela e renderizador");
	if(!SDL_CreateWindowAndRenderer(TITULO, LARGURA, ALTURA, 0, &janela, &renderizador)){
		SDL_Log(" -> Erro ao inicar a janela: %s \n",SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	return SDL_APP_CONTINUE;
}


static void carregarRGBA32(char *caminhoDaImagem){
	SDL_Log("Carregando imagem %s\n", caminhoDaImagem);
	superficie = IMG_Load(caminhoDaImagem);
	
	if(!superficie){
		SDL_Log(" -> Erro ao carregar imagem: %s\n",SDL_GetError);
		return;
	}
	
	SDL_Log("Convertendo superficie para o formato RGBA32..\n");
	SDL_Surface *convertido = SDL_ConvertSurface(superficie, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(superficie);
	if(!convertido){
		SDL_Log(" -> Erro ao converter superficie para o formato RGBA32: %s\n", SDL_GetError());
		superficie = NULL;
		return;
	}
	
	superficie = convertido;
	SDL_Log("Criando textura a partir da superficie...\n");
	textura = SDL_CreateTextureFromSurface(renderizador,superficie);
	if(!textura){
		SDL_Log(" -> Erro ao criar textura %s ", SDL_GetError());
		return;
	}
	
	SDL_GetTextureSize(textura,&retangulo.w, &retangulo.h ); //Voce vai pegar o tamnaho da imagem e escrever no w e h
}


static void inverterImagem(void){
	if(!superficie)
	{
		SDL_Log("Erro em inverterImagem(): Imagem INválida:\n");
		return;
	}
	
	SDL_LockSurface(superficie); //Você pode escrever  e e ler de uma superficie(Acesso direto)
	
	const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(superficie->format); //Pega as informaacoes(sao muitas(Apenas Leitura)
	const size_t total_pixels = superficie -> w * superficie -> h;
	
	Uint32 *pixels = (Uint32 *)superficie ->pixels;				//Uint32 = [0 - 4294967295]
	Uint8 r = 0;												//Uint8  = [0 - 255]
	Uint8 g = 0;	
	Uint8 b = 0;
	Uint8 a = 0;
	
	for(size_t i = 0; i < total_pixels;i++){
		SDL_GetRGBA(pixels[i],format,NULL, &r, &g, &b, &a); //Ele vai escrever o pixel nas variaveis rgba
		
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
		
		pixels[i] = SDL_MapRGBA(format, NULL, r,  g, b, a);	//Substirui o pixel da matriz
	}
	
	SDL_UnlockSurface(superficie);
	SDL_DestroyTexture(textura);
	textura = SDL_CreateTextureFromSurface(renderizador, superficie);
}



static void loop(){
	SDL_Event eventos;
	bool execucao = true;
	
	while(execucao == true){
		while(SDL_PollEvent(&eventos)){	//Percorre a lista de eventos
			switch(eventos.type){
				
			case SDL_EVENT_QUIT:
				execucao = false;
				break;
			}
		}
		
		SDL_SetRenderDrawColor(renderizador, 128, 128, 128, 255);
		SDL_RenderClear(renderizador);
		
		SDL_RenderTexture(renderizador, textura, NULL, &retangulo);
		
		SDL_RenderPresent(renderizador);
		
	}
}



int main(int argc, int *argv[]){	
	atexit(shutdown);

	//Inicializando o progama
	if(inicializar() == SDL_APP_FAILURE){
		return SDL_APP_FAILURE;
	}
	
	carregarRGBA32("assets/flor.jpg");
	
	if(retangulo.w > LARGURA || retangulo.h > ALTURA)
	{
		SDL_SetWindowSize(janela, retangulo.w, retangulo.h);
		SDL_SyncWindow(janela); //Não sei o que faz exatamente
			
	}
	
	inverterImagem();
	loop();
}
