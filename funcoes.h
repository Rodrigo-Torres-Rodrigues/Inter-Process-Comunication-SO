#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//mecanismo de pipe (pipe padrão unidirecional).
int pipeIPC() {
    //Criar o pipe
    int fd[2];
    pid_t pid = pipe(fd);

    printf("Criando o Pipe...");
    getchar();

    //Caso tenha algum erro
    if (pid == -1){
        perror("Erro ao criar pipe");
        exit(1);

    }else{
        printf("Pipe padrão feito com sucesso!");
        getchar();
    }


    pid = fork();

    if (pid < 0) {
        perror("Erro no fork");
        exit(1);
    }

    if (pid > 0) {
        // PROCESSO PAI
        close(fd[0]);       // fecha extremidade de leitura

        char mensagemUser[100];

        printf("\nDigite a mensagem para o processo filho:\n");
        scanf("%s", mensagemUser);

        write(fd[1], mensagemUser, strlen(mensagemUser) + 1);  // envia o texto ao filho
        close(fd[1]);       // fecha extremidade de escrita

    } else {
        // PROCESSO FILHO
        // fecha extremidade de escrita
        close(fd[1]);  

        char buffer[512];

        // lê o que o pai enviou e armazena em buffer.
        read(fd[0], buffer, sizeof(buffer));
        printf("\nProcesso filho recebeu a mensagem:\n%s\n", buffer);

        close(fd[0]);

        printf("\n\n*Pipe foi excluído*");
        getchar();
    }

    return 0;
}