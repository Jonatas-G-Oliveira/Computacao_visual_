#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>


int main(){
    printf("\n-> Inicializando SDL\n");

    atexit(SDL_Quit);
    //Inicializando;
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        printf("Erro ao inicar -> %s \n",SDL_GetError);
        return SDL_APP_FAILURE;
    };
    printf("-> SDL INICIALIZADO \n");
    

    printf("-> Criando Janela");
    const char *titulo = "JanelaSDL";
    const int largura = 600;
    const int altura = 200;
    SDL_Window *janela = SDL_CreateWindow(titulo,largura,altura,0);
    if(!janela){
        printf("Erro ao criar a janela-> %s \n",SDL_GetError);
        SDL_APP_FAILURE;
    }


    //Loop pra manter o programa aberto;
    SDL_Event evento;
    int execucao = 1;
    while(execucao == 1){
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_EVENT_QUIT){
                execucao = -1;
            }
        }
    }


    printf("Finalizando o programa");
    SDL_DestroyWindow(janela);
    janela = NULL;
    return 0;
}