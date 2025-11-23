#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "funcoes.h"

int main(){
    int num;

    printf("Digite o número correspondente:\n");
    printf("0 - Sair do programa\n");
    printf("1 - Pipes\n");
    printf("2 - Semáforos");

    scanf("%d", &num);
    getchar();

    system("clear || cls");

    switch(num){

        case 0:
        return 0;
        break;

        case 1:
        pipeIPC();
        break;

        case 2:
        semaforoSimulacao();
        break;
        default:
        printf("\nVocê digitou uma opção inexistente!!\n");
        getchar();

        system("clear || cls");
    }

    return main();
}
