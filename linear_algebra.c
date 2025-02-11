#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define TAMANHO 31


typedef struct {
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

void criarPonto(Ponto *p,int x,int y){
	p -> x = x;
	p -> y = y;
}

void inserirPontos(char **matriz,Ponto ponto){
	int origem = TAMANHO/2;
	
	int pontoX = origem + ponto.x;
	int pontoY = origem - ponto.y;
	
	matriz[pontoY][pontoX] = '*';
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

int multiplicacaoPorEscalar(Ponto A,int escalar,Ponto *resultado){
	resultado -> x = A.x * escalar;
	resultado -> y = A.y * escalar;
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


int main(){
	printf("Plano Cartesiano \n");
	char **plano_cartesiano;
	carregarPlano(&plano_cartesiano,TAMANHO);
	formatarPlano(plano_cartesiano,TAMANHO);
	
	Ponto pontoA;
	Ponto pontoB;
	Ponto pontoC;
	Ponto pontoD;
	Ponto pontoE;
	
	criarPonto(&pontoA,2,3);
	criarPonto(&pontoB,0,-2);
	
	inserirPontos(plano_cartesiano, pontoA);	
	//inserirPontos(plano_cartesiano, pontoB);
	
	//Operacoes com vetores
	subtracaoVetor(pontoA,pontoB,&pontoC);
	somaVetor(pontoA,pontoB,&pontoD);
	produtoEscalar(pontoA, pontoB);
	multiplicacaoPorEscalar(pontoB,2,&pontoE);
	moduloVetor(pontoB);
	
	//Tranformações
	translacao_2d(&pontoA,2,2);
	inserirPontos(plano_cartesiano,pontoA);

	imprimirPlano(plano_cartesiano,TAMANHO);
	//Free Matriz
	return 0;
}




