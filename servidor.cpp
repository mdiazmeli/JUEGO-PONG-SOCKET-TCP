#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <iostream>


#define PORT 7777 

using namespace std;

int p1 = 9;
int p2 = 9;
int jugadores[2];

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}
void *movi_1(void *apg)
{
    char buffer[256];
    while (true){    
        recv(jugadores[0], buffer, sizeof(buffer), 0);
        p1 = atoi(buffer);
        usleep(5000);
    }
}
void *movi_2(void *apg)
{
    char buffer[256];
    while (true){    
        recv(jugadores[1], buffer, sizeof(buffer), 0);
        p2 = atoi(buffer);
        usleep(5000);
    }
}

void *write_1(void *apg)
{
       while (true){
            string s = to_string(p2);
            char mensaje[2];
            strcpy(mensaje,s.c_str());
            write(jugadores[0], mensaje, sizeof(mensaje));
            usleep(50000);
     }
}
void logo()
{
    system("clear");
    gotoxy(33,1); printf("███████████████████");
    gotoxy(33,2); printf("█                 █");
    gotoxy(33,3); printf("█          ██████ █");
    gotoxy(33,4); printf("█          ██████ █");
    gotoxy(33,5); printf("█          ██████ █");
    gotoxy(33,6); printf("█ █████████       █");
    gotoxy(33,7); printf("█ █████████       █");
    gotoxy(33,8); printf("█  ████████       █");
    gotoxy(33,9); printf("██  ███████      ██");
    gotoxy(33,10);printf(" ██  ██████     ██");
    gotoxy(33,11);printf("  ██   ████    ██");
    gotoxy(33,12);printf("   ██         ██");
    gotoxy(33,13);printf("    ███████████");
    gotoxy(33,14);printf("       UTEM    ");
    printf("\n");
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, socket_client, valread; 
    struct sockaddr_in direccion;
    struct sockaddr_in client_addr;
    fd_set descriptoresLectura; 
    socklen_t size_addr = 0;
    int opt = 1; 
 
    int addrlen = sizeof(direccion); 

    logo();
    //:::::::::::::::::::::::::Creacion del socket::::::::::::::::::::::::::::::::::::::
    // Crear descriptor de archivo de socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("fallo creacion de Socket"); 
        exit(EXIT_FAILURE); 
    }
    printf("Esperando jugadores..........\n");   
    // Conexión forzada del zócalo al puerto 7777
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    direccion.sin_family = AF_INET; 
    direccion.sin_addr.s_addr = INADDR_ANY; 
    direccion.sin_port = htons( PORT ); 
       
    // Conexión forzada del zócalo al puerto 7777 
    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion))<0) 
    { 
        perror("Fallo el enlace"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 2) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 


    socket_client = accept(server_fd, (struct sockaddr *)&direccion,(socklen_t*)&addrlen);
    jugadores[0]= socket_client;
    if (socket_client == -1) 
    { 
        perror("error");
    }
    printf("\nSe ha conectado el jugador 1\n");
    char player1[2] = "1";
    write(jugadores[0], player1,sizeof(player1));
    usleep(1000);
    char msg[20]="Esperando jugador 2";
    write(jugadores[0],msg,sizeof(msg));
    //---------------------------
    socket_client = accept(server_fd, (struct sockaddr *)&direccion,(socklen_t*)&addrlen);
    jugadores[1]= socket_client;
    if (socket_client  == -1) 
    { 
        perror("error");
    }
    printf("\nSe ha conectado el jugador 2\n");
    char player2[2] = "2";
    write(jugadores[1], player2,sizeof(player2));
    usleep(1000);
    char msg2[29]="Se ha conectado el jugador 2";
    for (int i = 0; i < 2; ++i)
    {
        write(jugadores[i],msg2,sizeof(msg2));
    }
    usleep(3500000);


    pthread_t hear1,hear2,env1;

    if(pthread_create(&hear1, NULL, &movi_1, NULL)) {
        fprintf(stderr, "Error creating thread\n");
    }
    if(pthread_create(&hear2, NULL, &movi_2, NULL)) {
        fprintf(stderr, "Error creating thread\n");
    }
    if(pthread_create(&env1, NULL, &write_1, NULL)) {
        fprintf(stderr, "Error creating thread\n");
    }

    while(true)
    {
        string s = to_string(p1);
        char mensaje[2];
        strcpy(mensaje,s.c_str());
        write(jugadores[1], mensaje, sizeof(mensaje));
        usleep(50000);
    }

    close(server_fd);
    close(socket_client);       
    
    if(pthread_join(hear1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
    }
    if(pthread_join(hear2, NULL)) {
        fprintf(stderr, "Error joining thread\n");
    }
    if(pthread_join(env1, NULL)) {
        fprintf(stderr, "Error joining thread\n");
    }

    return EXIT_SUCCESS; 
} 
