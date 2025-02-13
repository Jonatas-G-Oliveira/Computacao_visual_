#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#define TAMANHO 31

int qtd_pontos = 0;

typedef struct {
	char nome[1];
	int x;
	int y;
}Ponto;


void alocarMatriz(int ***matriz,int linha,int coluna){
	int **m = calloc(linha ,sizeof(int *));
	int i = 0;
	for(i = 0;i < linha ;i++){
		m[i] = calloc(coluna , sizeof(int));
	}
	*matriz = m;
}
/*
void alocarVetor(Ponto *vetor){
	Ponto *v = (Ponto *)calloc(10 , sizeof( Ponto));
	vetor = v;
}*/

//Faz o plano cartesiano(Vai ser sempre quadrado).
void carregarPlano(char ***matriz,int tamanho){
	char **m = calloc(tamanho ,sizeof(char *));
	int i = 0;
	for(i = 0;i < tamanho;i++){
		m[i] = calloc(tamanho , sizeof(char));
	}
	*matriz = m;
}


void formatarPlano(char **matriz,int tamanho){	
	int meio = tamanho/2 ;
	int linha,coluna = 0;
	for(linha = 0; linha < tamanho; linha++){
		for(coluna = 0;coluna < tamanho; coluna++){
			matriz[linha][meio] = '.';
			matriz[meio][coluna] = '-';
		}
	}
}

//Preciso alocar memoria para trabalhar com struct por referencia
void criarPonto(Ponto *p,char nome,int x,int y){
	p -> x = x;
	p -> y = y;
	strcpy(p -> nome,&nome);
}

void inserirPontos(char **matriz,Ponto *vetor_pontos,Ponto ponto){
	int origem = TAMANHO/2;

	//Estou removendo o ponto anterior
	int i,p,ja_existe = 0;

	for(i = 0;i < qtd_pontos;i++){
		if(strcmp(vetor_pontos[i].nome,ponto.nome) == 0){
			int pontoX = origem + vetor_pontos[i].x;
			int pontoY = origem - vetor_pontos[i].y;
			matriz[pontoY][pontoX] = ' ';
			p = i;
			ja_existe = 1;
		}
	}
	//Adicionanndo um novo ponto
	int pontoX = origem + ponto.x;
	int pontoY = origem - ponto.y;	
	matriz[pontoY][pontoX] = '*';
	
	//Atualizando o vetor de pontos com as novas coordeanas 
	if(ja_existe == 1){
		strcpy(vetor_pontos[p].nome,ponto.nome);
		vetor_pontos[p].x =  ponto.x;
		vetor_pontos[p].y =  ponto.y;
		ja_existe = 0;
	}else{
		strcpy(vetor_pontos[qtd_pontos].nome,ponto.nome);
		vetor_pontos[qtd_pontos].x =  ponto.x;
		vetor_pontos[qtd_pontos].y =  ponto.y;
		qtd_pontos++;
	}
	

	
}

//Você vai gerar um novo vetor centrado na origem
void subtracaoVetor(Ponto A,Ponto B,Ponto *C){
	C -> x =  B.x - A.x;
	C -> y =  B.y - A.y;
}

void somaVetor(Ponto A,Ponto B,Ponto *C){
	C -> x = A.x + B.x;
	C -> y = A.y + B.y;
}

int produtoEscalar(Ponto A,Ponto B){
	return A.x * B.x + A.y * B.y;
}

int multiplicacaoPorEscalar(Ponto *A,int escalar){
	A -> x = A -> x * escalar;
	A -> y = A -> y * escalar;
}

float moduloVetor(Ponto ponto){
	float resultado = sqrt(pow(ponto.x,2) + pow(ponto.y,2));
	return resultado;
}

void imprimirPlano(char **matriz,int tamanho){
	int i,j = 0;
	for(i = 0; i < tamanho; i++){
		for(j = 0; j < tamanho; j++){
			printf("%c ",matriz[i][j]);
		}
		printf("\n");
	}	
}

/*EM MANUTENCAO
float anguloVetores(Ponto vetorA,Ponto vetorB){
	int numerador = produtoEscalar(vetorA,vetorB);
	float denominador = moduloVetor(vetorA) * moduloVetor(vetorB);
	
	return numerador / denominador;
}
*/

void multiplicacao_matrizes(int **matrizA,int linhaA,int colunaA,int **ponto,int colunaB,int ***resultado){
	int i,j,k = 0;
	int soma = 0;
	int **r;
	alocarMatriz(&r,linhaA,colunaB);
	
	for(i = 0; i < linhaA;i++){
		for(j = 0; j < colunaA;j++){
			for(k = 0; k < colunaB;k++){
				soma =  matrizA[i][j] * ponto[j][k];
				r[i][k] += soma;
			}		
		}
	}
	
	*resultado = r;
}

// TRANSFORMACOES 2D
void translacao_2d(Ponto *ponto,int deslocamentoX,int deslocamentoY){
	int **matriz_deslocamento;
	int **matriz_homogenea;
	int **resultado;

	alocarMatriz(&matriz_deslocamento,3,3);
	alocarMatriz(&matriz_homogenea,3,1);
	alocarMatriz(&resultado,3,1);
	int linha,coluna = 0;
	
	//Prepara a matriz para a multiplicacao
	matriz_deslocamento[0][2] = deslocamentoX;
	matriz_deslocamento[1][2] = deslocamentoY;
	for(linha = 0;linha < 3;linha++){
		for(coluna  = 0;coluna < 3;coluna++){
			if(linha == coluna){
				matriz_deslocamento[linha][coluna] = 1;
			}
		}
	}
	
	//Tranformando o ponto em uma coordenada homogenea
	matriz_homogenea[0][0] = ponto -> x;
	matriz_homogenea[1][0] = ponto -> y;
	matriz_homogenea[2][0] = 1;
	
	//Retornando o novo ponto
	multiplicacao_matrizes(matriz_deslocamento,3,3,matriz_homogenea,1,&resultado);
	ponto -> x = resultado[0][0];
	ponto -> y = resultado[1][0];
}

//Sem Matrz e sem homogeneidade ,não funciona tão bem
void rotacao_2d(Ponto *ponto,int angulo){	
	int rX = cos(angulo) * ponto -> x  -  ponto -> y * sin(angulo);
	int rY = ponto -> x  * sin(angulo) +  ponto -> y * cos(angulo);
	
	ponto -> x = rX;
	ponto -> y = rY;
}


int main(){
	printf("Plano Cartesiano \n");
	char **plano_cartesiano;
	Ponto vetor_pontos[10];
	//alocarVetor(vetor_pontos);
	carregarPlano(&plano_cartesiano,TAMANHO);
	formatarPlano(plano_cartesiano,TAMANHO);
	
	Ponto pontoA;
	Ponto pontoB;
	Ponto pontoC;
	Ponto pontoD;
	Ponto pontoE;
	Ponto pontoF;
	Ponto pontoG;
	Ponto pontoH;	
	
	criarPonto(&pontoA,'A',2,4);
	criarPonto(&pontoB,'B',3,4);
	criarPonto(&pontoC,'C',4,4);
	criarPonto(&pontoD,'D',2,5);
	criarPonto(&pontoE,'E',4,5);
	criarPonto(&pontoF,'F',3,6);
	criarPonto(&pontoG,'G',3,7);
	//criarPonto(&pontoH,'H',5,3);
	
	inserirPontos(plano_cartesiano,vetor_pontos,pontoA);
	inserirPontos(plano_cartesiano,vetor_pontos,pontoB);
	inserirPontos(plano_cartesiano,vetor_pontos,pontoC);	
	inserirPontos(plano_cartesiano,vetor_pontos,pontoB);
	inserirPontos(plano_cartesiano,vetor_pontos,pontoC);
	inserirPontos(plano_cartesiano,vetor_pontos,pontoD);
	inserirPontos(plano_cartesiano,vetor_pontos,pontoE);
	inserirPontos(plano_cartesiano,vetor_pontos,pontoF);

	

	
	//Ao invés de mexer ponto por ponto eu poderioa ter um vetor imagem armazenado os pixels
	//Lopp de exibicao para translacao
	imprimirPlano(plano_cartesiano,TAMANHO);
	Sleep(1000);
	
	int i = 0;
	int angulo = 30;
	for(i = 0;i < 10;i++){
		imprimirPlano(plano_cartesiano,TAMANHO);	
		/*
		rotacao_2d(&pontoA,angulo);
		rotacao_2d(&pontoB,angulo);
		rotacao_2d(&pontoC,angulo);
		rotacao_2d(&pontoD,angulo);
		rotacao_2d(&pontoE,angulo);
		rotacao_2d(&pontoF,angulo);
	
		inserirPontos(plano_cartesiano,vetor_pontos,pontoA);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoB);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoC);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoD);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoE);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoF);
		*/
		
		translacao_2d(&pontoA,0,1);
		translacao_2d(&pontoB,0,1);
		translacao_2d(&pontoC,0,1);
		translacao_2d(&pontoD,0,1);
		translacao_2d(&pontoE,0,1);
		translacao_2d(&pontoF,0,1);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoA);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoB);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoC);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoD);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoE);
		inserirPontos(plano_cartesiano,vetor_pontos,pontoF);
		
		
		
		Sleep(100);
	}
	
	//Free Matriz
	return 0;
}




