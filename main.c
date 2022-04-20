#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define NTHREAD 2
#define TAMANHO_VETOR 10000

int meuVetor[TAMANHO_VETOR]; // Vetorzão (é global mesmo :( )


void *mudarVetor(void *arg){
    int idThread = (int)(intptr_t) arg; // convertendo o ponteiro para inteiro
    /* (intptr_t) é necessário para a conversão porque o ponteiro pra void tem 8 bytes(em uma máquina de 64 bits),
     enquanto um inteiro tem apenas 4. Se eu fizer uma conversão direta, além do warning do compilador, eu também posso perder bits. */
    
    int posicaoInicial, posicaoFinal;

    if(idThread == 0) { // Se for a prineira thread, faz apenas primeira metade
        posicaoInicial = 0;
        posicaoFinal = TAMANHO_VETOR / NTHREAD;
    }
    else if(idThread == 1){ // Se for a prineira thread, faz apenas segunda metade
        posicaoInicial = TAMANHO_VETOR /NTHREAD;
        posicaoFinal = TAMANHO_VETOR;
    }

    for(int i = posicaoInicial;i < posicaoFinal; i++){
        meuVetor[i] = i*i; // calcula o quadrado de i e coloca na posicao i do vetor
    }
   
    pthread_exit(NULL);
}


void printVetor(int vetor[TAMANHO_VETOR]){
    for(int i = 0; i < TAMANHO_VETOR; i++ ){
        printf("%d -> %d\n",i,vetor[i]);
    }    
}

void assertVector(int vetor[TAMANHO_VETOR]){
    for(int i = 0; i < TAMANHO_VETOR; i++ ){
        if(vetor[i] != i*i){
            printf("Erro encontrado na posição %d!\n",i);
        }
    }
    printf("O vetor foi verificado com sucesso!\n");
}

int main(){

    // identificadores das threads
    pthread_t identificadores[NTHREAD]; 
    
    int thread;

    for(thread = 0; thread < NTHREAD; thread++){
       
       // explicação do (intptr_t) acima
        if(pthread_create(&identificadores[thread], NULL, mudarVetor, (void*) (intptr_t) thread) ){
            exit(-1); // Deu ruim
        }
   

    }

    for(thread = 0; thread < NTHREAD;  thread++){
        if (pthread_join(identificadores[thread], NULL)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1); 
        }
    }
    assertVector(meuVetor);
    pthread_exit(NULL);

    return 0;
}
