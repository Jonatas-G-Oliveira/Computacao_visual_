#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

int main(int argc,int *argv[]){

    atexit(SDL_Quit);

    if(!SDL_Init(SDL_INIT_VIDEO)){
        printf("Erro ao inicializar o SDL %s",SDL_GetError);
        return SDL_APP_FAILURE;
    }

    const char* TITULO = "Janela SDL";
    enum constants{
        LARGURA = 640,
        ALTURA = 480,
        TAMANHO_MAX = 64,
    };

    SDL_Window *janela = NULL;
    SDL_Renderer *renderizador = NULL;

    if(!SDL_CreateWindowAndRenderer(TITULO,480,640,0,&janela,&renderizador)){
        printf("Erro ao criar a janela %s",SDL_GetError);
        return SDL_APP_FAILURE;
    }

    //Carregando Imagem
    SDL_Texture *ImageBitmap = IMG_LoadTexture(renderizador,"test.bmp");
    if(!ImageBitmap){
        SDL_Log("Erro ao carregar a imagem: %s\n",SDL_GetError());
    }

    SDL_FRect retangulo = {
        .x = 0.0f,
        .y = 0.0f,
        .w = ImageBitmap ? ImageBitmap->w : 0.0f,//Se a imagem for carregada eu uso a largura da imagem
        .h = ImageBitmap ? ImageBitmap->h : 0.0f,//Se a imagem for carregada eu pego oa altura dela
    };



    //Loop
    
    char buffer[TAMANHO_MAX] = { 0 };
    SDL_Event evento;
    int execucao = 1;
    while(execucao == 1){
        while(SDL_PollEvent(&evento)){
            switch (evento.type)
            {

            case SDL_EVENT_QUIT:
                execucao = 0;
                break;

            case SDL_EVENT_MOUSE_MOTION:
                snprintf(buffer,TAMANHO_MAX,"%s (%.0f %.0f)",TITULO, evento.motion.x,evento.motion.y);
                SDL_SetWindowTitle(janela,buffer);
                break;
            
            }
        }
        //O ultimo parametro é onde eu vou exibir
        SDL_RenderTexture(renderizador,ImageBitmap,NULL,&retangulo);
        SDL_RenderPresent(renderizador);
    }
    

    SDL_DestroyWindow(janela);
    SDL_DestroyRenderer(renderizador);

}