/*
Trabalho da travessia dos babuinos 
Feito em c num computador com sistema operacional:
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 22.04.1 LTS
Release:        22.04
Codename:       jammy
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


int babuinos_cruzando = 0;
int direcao = 0; // 0 para esquerda, 1 para direita

void *babuino(void *arg) {
    int *v = (int*) arg;
    int minha_direcao = v[0];
    int bab = v[1];

    printf("Babuino %d esperando para ir a %s\n",bab,minha_direcao == 0 ? "esquerda" : "direita");

// Verifica se o babuino estanona direcao dos demais, se estiver fica na espera ate todos cruzarem e enviarem o sinal
// temos duas verificacoes de babuinos cruzando
    pthread_mutex_lock(&mutex);
    while (minha_direcao != direcao) {
        while (babuinos_cruzando != 0){
            pthread_cond_wait(&cond, &mutex);
        }
        direcao = minha_direcao;
    }

    babuinos_cruzando++;

    printf("Babuino -  %d indo para a %s\n",bab, minha_direcao == 0 ? "esquerda" : "direita");

    pthread_mutex_unlock(&mutex);

    sleep(rand() % 2); // Simula o tempo que leva para cruzar

    printf("Babuino -  %d  ",bab);
    printf("chegou à %s\n", minha_direcao == 0 ? "esquerda" : "direita");

    babuinos_cruzando--;

// Permite apenas que o sinal seja enviado quando tiver todos os babuinos da mesma direcao cruzado
    pthread_mutex_lock(&mutex);
    if (babuinos_cruzando == 0) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
   

    int num_threads = atoi(argv[1]);

    pthread_t babuinos[num_threads];
    int j = 0;

    for (int i = 0; i < num_threads; i++) {
        j = rand() % 2;
        int *v = calloc(2, sizeof(int));
        if(i == 0){
            j = 0;
        }
        v[0] = j;
        v[1] = i;
        sleep(rand() % 2);
        pthread_create(&babuinos[i], NULL, babuino, (void*)v);
    }


    for (int i = 0; i < num_threads; i++) {
        pthread_join(babuinos[i], NULL);
    }

    return 0;
}
