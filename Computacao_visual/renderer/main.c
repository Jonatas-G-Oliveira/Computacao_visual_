#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

void shutdown(){
    SDL_Log("Shutdown()\n");
    SDL_Quit();
}

int main(int argc,char *argv[]){
    printf("\n-> Inicializando SDL\n");
    atexit(shutdown);

    //Video Starter
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        printf("Erro ao inicializar o vídeo: %s",SDL_GetError);
        return SDL_APP_FAILURE;
    };

    //Ao invés de criar apenas uma janela,eu crio
    SDL_Window *janela = NULL;
    SDL_Renderer *renderizador = NULL;

    if(!SDL_CreateWindowAndRenderer ("Render_Window",680,480,0,&janela,&renderizador)){
        printf("Erro ao inicializar a janela: %s",SDL_GetError);
        return SDL_APP_FAILURE;
    }

    char buffer[64] = { 0 };
    //Looping
    SDL_Event evento;
    int execucao = true;
    while(execucao == true){
        while(SDL_PollEvent(&evento)){
            switch (evento.type)
            {
                case SDL_EVENT_QUIT:
                    execucao = false;
                    break;

                //Estou concatenndo titulo,posX,posY e jogando no buffer,depois atualizando a jnela
                case SDL_EVENT_MOUSE_MOTION:
                    snprintf(buffer,64,
                    "%s (%.0f %.0f)","Janela",evento.motion.x,evento.motion.y);
                    SDL_SetWindowTitle(janela,buffer);
                    break;
            }
        }


        //Define Cor de desenho
        SDL_SetRenderDrawColor(renderizador, 128, 128, 128, 255);
        //Limpar o espaço da memória,com a cor que a gente definiu
        SDL_RenderClear(renderizador);
       
        //Define uma cor e desenha na tela
        SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
        SDL_RenderLine(renderizador, 10, 240 - 10 ,680 - 10,240 -10);

        SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 255);
        SDL_RenderLine(renderizador, 10, 240  ,680 - 10,240);

        SDL_SetRenderDrawColor(renderizador, 0, 0, 255, 255);
        SDL_RenderLine(renderizador, 10, 240 + 10 ,680 - 10,240 + 10);
        
        //Atualiza a tela com as alterações feitas até o momento!
        SDL_RenderPresent(renderizador);
        
    }


    SDL_DestroyWindow(janela);
    SDL_DestroyRenderer(renderizador);
    return 0;
}