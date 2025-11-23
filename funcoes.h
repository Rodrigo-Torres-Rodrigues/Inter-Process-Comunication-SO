#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

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

        //Processo pai espera até o filho terminar.

        wait(NULL);

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



// Semáforo global
sem_t semaforo;

// Função que cada thread irá executar
void* processo(void* arg) {
    int id = *((int*)arg);  // identifica qual "processo" é

    while (1) {
        printf("Processo %d: tentando acessar o recurso...\n", id);

        // sem_wait = operação DOWN
        // Se o semáforo está 1 → passa e vira 0
        // Se está 0 → a thread fica BLOQUEADA pelo sistema
        sem_wait(&semaforo);

        printf("Processo %d: entrou na seção crítica (utilizando recursos)\n", id);

        // Simula uso do recurso
        sleep(2);

        printf("Processo %d: saindo da seção crítica\n\n", id);

        // sem_post = operação UP
        // Libera o recurso → contador volta a 1
        sem_post(&semaforo);

        // Apenas uma pausa antes de tentar novamente
        sleep(4);
    }

    return NULL;
}

int semaforoSimulacao() {
    pthread_t t1, t2;   // duas "threads" = dois "processos"
    int id1 = 1, id2 = 2;

    // Inicializa o semáforo com valor 1 (semáforo binário)
    sem_init(&semaforo, 0, 1);

    // Cria duas threads que competirão pelo mesmo recurso
    //O processo é a função void
    pthread_create(&t1, NULL, processo, &id1);
    pthread_create(&t2, NULL, processo, &id2);

    // Mantém o programa rodando enquanto as threads executam
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroi o semáforo (nunca será alcançado nesse exemplo)
    sem_destroy(&semaforo);

    return 0;
}
