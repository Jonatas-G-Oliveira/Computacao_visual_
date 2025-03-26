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
static const char PATH[50] = "assets/kodim23.png";

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
static SDL_FRect retangulo = { .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f};

// -------------------------------------------
// Declaracao de funcoes
// -------------------------------------------

static void carregarRGBA32();

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


static void carregarRGBA32(){
	SDL_Log("Carregando imagem %s\n", PATH);
	superficie = IMG_Load(PATH);
	
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
	
	//Pra exibir na tela voce precisa converter da superficie para textura,O renderizador e responsavel pela exibicao
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

static void tonsCinzaMedia(void){
	if(!superficie)
	{
		SDL_Log("Erro em tonsCinzaMedia(): Imagem Invalida\n");
		return;	
	}
	
	const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(superficie -> format);
	SDL_LockSurface(superficie); //"Destravando" a superficie que eu vou manipular
	
	const size_t total_pixels = superficie -> w * superficie -> h; //Tamanho do vetor de pixels 
	Uint32 *pixels = (Uint32 *)superficie -> pixels; 				// Vetor de pixels da imagem
	
	
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 0;
	
	for(size_t p = 0; p < total_pixels; p++){
		SDL_GetRGBA(pixels[p],format,NULL, &r, &g, &b, &a);
		
		int	m = (r + g + b) / 3;
		
		pixels[p] = SDL_MapRGBA(format, NULL, m, m, m, a);
	}
	
	SDL_UnlockSurface(superficie);
	SDL_DestroyTexture(textura);
	textura = SDL_CreateTextureFromSurface(renderizador, superficie);
	
}

static void tonsCinzaEquacao(void){
	if(!superficie)
	{
		SDL_Log("Erro em tonsCinzaEquacao(): Imagem Invalida\n");
		return;	
	}
	
	const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(superficie -> format);
	SDL_LockSurface(superficie);
	
	const size_t total_pixels = superficie -> w * superficie -> h;
	Uint32 *pixels = (Uint32 *)superficie -> pixels;
	
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 0;
	
	for(size_t p = 0; p < total_pixels; p++){
		SDL_GetRGBA(pixels[p], format, NULL, &r, &g, &b, &a);
		 
		 int m = r * 0.2126 + g * 0.7152 + b * 0.0722;
		 
		 pixels[p] = SDL_MapRGBA(format, NULL, m, m, m, a);
	}
	
	SDL_UnlockSurface(superficie);
	SDL_DestroyTexture(textura);
	textura = SDL_CreateTextureFromSurface(renderizador, superficie);
	
}


static void RGBAhistogram(){
	SDL_FRect histograma = {.x = 0.0f, .y = 0.0f, .w = 260, .h = 200 };
	//Carregando histograma
	SDL_SetRenderDrawColor(renderizador, 255, 255, 255,255);
	SDL_RenderFillRect(renderizador,&histograma);
	
	//Esse vetores sao responsaveis por guardar a quantidade de pixels que
	//possuem uma determinada intesiDade que vai de 0 a 255
	int RED[256], GREEN[256], BLUE[256];
	for(int i = 0; i < 256; i++){
		RED[i] = 0;
		GREEN[i] = 0;
		BLUE[i] = 0;
	}
	
	//Informacoes da imagem
	const Uint32 *vetor_pixels =  (Uint32 *) superficie -> pixels;
	const size_t total_pixels = superficie -> w * superficie -> h;
	const SDL_PixelFormatDetails *formato = SDL_GetPixelFormatDetails(superficie -> format);
	
	
	//Percorrendo a imagem
	SDL_LockSurface(superficie);
	Uint8 r, g, b, a = 0;
	for(size_t i = 0; i < total_pixels; i++){
		//Cada valor intensidade tera uma quantidade de pixels correspondente
		SDL_GetRGBA(vetor_pixels[i], formato, NULL, &r, &g, &b, &a);	
		RED[r] += 1;
		GREEN[g] += 1;
		BLUE[b] += 1;
	}
	SDL_UnlockSurface(superficie);
	
	//Obtendo pontos
	SDL_FPoint pontosRED[256];
	SDL_FPoint pontosBLUE[256];
	SDL_FPoint pontosGREEN[256];
	
	for(size_t i = 0; i < 256; i++){
		if(i == 0){
			pontosRED[i].x = 0;
			pontosGREEN[i].x = 0;
			pontosBLUE[i].x = 0;
		}else{
			//Deslocando uma posicao pro lado
			pontosRED[i].x = pontosRED[i-1].x + 1;
			pontosGREEN[i].x = pontosGREEN[i-1].x + 1;
			pontosBLUE[i].x = pontosBLUE[i-1].x + 1;
		}
		pontosRED[i].y = 200 - (RED[i] / 50);
		pontosGREEN[i].y = (200 - GREEN[i] / 50);
		pontosBLUE[i].y = (200 - BLUE[i] / 50);
	}
	
	
	//Renderizando linhas
	
	for(int i = 0; i < 256;i++){
		SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 0);
		SDL_RenderLine(renderizador,pontosRED[i].x , 200, pontosRED[i].x,pontosRED[i].y );
	}
	
	for(int i = 0; i < 256;i++){
		SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 0);
		SDL_RenderLine(renderizador,pontosRED[i].x , 200, pontosGREEN[i].x,pontosGREEN[i].y );
	}
	for(int i = 0; i < 256;i++){
		SDL_SetRenderDrawColor(renderizador, 0, 0, 255, 0);
		SDL_RenderLine(renderizador,pontosRED[i].x , 200, pontosBLUE[i].x,pontosBLUE[i].y );
	}
}


static void loop(){
	int resposta;
	printf("[1] INVERTER IMAGEM | [2]TONS DE CINZA(MEDIA) | [3] TONS DE CINZA(EQUACAO) | [0] IMAGEM ORIGINAL\n");
	
	SDL_Event eventos;
	bool execucao = true;
	while(execucao == true){
		while(SDL_PollEvent(&eventos)){	//Percorre a lista de eventos
			switch(eventos.type){
				
			case SDL_EVENT_QUIT:
				execucao = false;
				break;
			
			
			case SDL_EVENT_KEY_DOWN:
				if(eventos.key.key == SDLK_0 && !eventos.key.repeat){
					carregarRGBA32(PATH);
				}
				if(eventos.key.key == SDLK_1 && !eventos.key.repeat){
					inverterImagem();
				}
				if(eventos.key.key == SDLK_2 && !eventos.key.repeat){
					tonsCinzaMedia();
				}
				if(eventos.key.key == SDLK_3 && !eventos.key.repeat){
					tonsCinzaEquacao();
				}
				break;
			}	
		}
	
		//Preenchendo o fundo
		SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
		SDL_RenderClear(renderizador);
		
		//Carregando imagem
		SDL_RenderTexture(renderizador, textura, NULL, &retangulo);
		
		RGBAhistogram();
		
		SDL_RenderPresent(renderizador);
		SDL_Delay(100);
	}
	
}


int main(int argc, int *argv[]){	
	atexit(shutdown);

	//Inicializando o progama
	if(inicializar() == SDL_APP_FAILURE){
		return SDL_APP_FAILURE;
	}

	carregarRGBA32();
	
	//Se a textura for maior que a altura,redimensiona a janela.
	if(retangulo.w > LARGURA || retangulo.h > ALTURA)
	{
		SDL_SetWindowSize(janela, retangulo.w, retangulo.h);
		SDL_SyncWindow(janela); //Não sei o que faz exatamente
			
	}

	loop();
}
