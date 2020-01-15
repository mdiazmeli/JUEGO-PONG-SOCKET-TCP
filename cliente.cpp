#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h> 
#include <stdlib.h>
#include "iostream"



#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <netdb.h>

using namespace std;

#define PORT 7777 

int sockServ,fin = 0; 

char getch2(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}
//campo...................
void campo()
{
    system("clear");
    int i=0;
    while(i<81){
        gotoxy(i,0);
        printf("█");
        gotoxy(i,24);
        printf("█");
        i++;
    
    }
    i=1;
    while(i<24){
        gotoxy(0,i);
        printf("█");
        gotoxy(80,i);
        printf("█");
        i++;
    }
}
class JUGADOR
{
    int x,y;
public:
     JUGADOR(int _x,int _y);
    ~ JUGADOR();
    void pintar() const;
    void borrar() const;
    void mover(int _y);
    int RX(){return x;}
    int RY(){return y;}
    void m_oponente(int _y){borrar();y = _y;pintar();}
};
JUGADOR::JUGADOR(int _x,int _y){
    x=_x, y=_y;
}
JUGADOR::~JUGADOR(){}

void JUGADOR::pintar() const{
    gotoxy(x,y);   printf("█");
    gotoxy(x,y+1); printf("█");
    gotoxy(x,y+2); printf("█");
    gotoxy(x,y+3); printf("█");
    gotoxy(x,y+4); printf("█");
    gotoxy(x,y+5); printf("█");
printf("\e[?25l");
}
void JUGADOR::borrar() const{
    gotoxy(x,y); printf(" ");
    gotoxy(x,y+1); printf(" ");
    gotoxy(x,y+2); printf(" ");
    gotoxy(x,y+3); printf(" ");
    gotoxy(x,y+4); printf(" ");
    gotoxy(x,y+5); printf(" ");
printf("\e[?25l");
}
void JUGADOR::mover(int _y){
    borrar();
    y += _y;
    pintar();
}
class PELOTA
{
    int x,y;
    int dx,dy;
public:
    PELOTA(int _x,int _y,int _dx,int _dy);
    ~PELOTA();
    void pintar()const;
    void borrar()const;
    void mover(JUGADOR a,JUGADOR b);
    void asign(int _x,int _y){x = _x; y = _y;}
};

PELOTA::PELOTA(int _x,int _y,int _dx,int _dy){
    x=_x, y=_y,dx=_dx,dy=_dy;
}
PELOTA::~PELOTA(){}
void PELOTA::pintar()const{
    gotoxy(x,y); printf("■");printf("\e[?25l");
}
void PELOTA::borrar()const{
    gotoxy(x-1,y); printf("   ");printf("\e[?25l");
}
void PELOTA::mover(JUGADOR a, JUGADOR b){
    borrar();
    x +=dx;
    y +=dy;
    pintar();
    if(x + dx == 1 || x + dx == 79)
    {
        borrar();
        asign(39,11);
        dx =-dx;
        fin++;
    }
    if(y + dy == 1 || y + dy == 24) 
        dy = -dy;//Bordes campo
    if(x + dx == a.RX() && y >= a.RY()   && y <= a.RY() + 1 ) {dx = 1; dy = -1;}
    if(x + dx == a.RX() && y >= a.RY()+2 && y <= a.RY() + 3 ) {dx = 1; dy =  0;}
    if(x + dx == a.RX() && y >= a.RY()+4 && y <= a.RY() + 5 ) {dx = 1; dy =  1;}
    if(x + dx == b.RX() && y >= b.RY()   && y <= b.RY() + 1 ) {dx =-1; dy = -1;}
    if(x + dx == b.RX() && y >= b.RY()+2 && y <= b.RY() + 3 ) {dx =-1; dy =  0;}
    if(x + dx == b.RX() && y >= b.RY()+4 && y <= b.RY() + 5 ) {dx =-1; dy =  1;}
}
JUGADOR a(6,9);
JUGADOR b(74,9);

void logo()
{
    system("clear");

    gotoxy(33,8); printf(" ██████  ██  ██    ██");
    gotoxy(33,9); printf(" ██          ███   ██ ");
    gotoxy(33,10);printf(" █████   ██  ██ █  ██ ");
    gotoxy(33,11);printf(" ██      ██  ██  █ ██ ");
    gotoxy(33,12);printf(" ██      ██  ██   ███");
    gotoxy(33,13);printf(" ██      ██  ██    ██");
    gotoxy(33,17);printf("    presione ENTER");
    printf("\n");
}
void *mov_1(void *apg){
    while (fin<3){
        char c = getch2();
        if (c == 'w' && a.RY()>2){
            a.mover(-1);
        }
        else if(c == 's' && a.RY()<18){
            a.mover(1);
        }   
        string s = to_string(a.RY());
        char mensaje[2];
        strcpy(mensaje,s.c_str());
        
        send(sockServ, &mensaje, sizeof(&mensaje), 0);
    }

}
void *mov_2(void *apg){
    while (fin<3){
        
        char c = getch2();
        if (c == 'w' && b.RY()>2){
            b.mover(-1);
        }
        else if(c == 's' && b.RY()<18){
            b.mover(1);
        }

        string s = to_string(b.RY());
        char mensaje[2];
        strcpy(mensaje,s.c_str());
        
        send(sockServ, mensaje, sizeof(mensaje), 0);
    }
}
void *recv_1(void *apg){
    while (fin<3){
        a.pintar();
        b.pintar();
        int aux;
        int n = 0;
        char buffer[256];
        if ((n = recv(sockServ, buffer, sizeof(buffer), 0)) > 0) {
            aux =  atoi(buffer);
            a.m_oponente(aux);
            usleep(50000);
        }
    }
    logo();
}
void *recv_2(void *apg){
    while (fin<3){
        a.pintar();
        b.pintar();
        int aux;
        int n = 0;
        char buffer[256];
        if ((n = recv(sockServ, buffer, sizeof(buffer), 0)) > 0) {
            aux =  atoi(buffer);
            b.m_oponente(aux);
            usleep(50000);
        }
    }
    logo();
}
int main(int argc, char const *argv[]) 
{ 

    //--------------------------Variables del Servidor---------------------------------
    int socketCliente;
    struct sockaddr_in direccion;  
 
    //:::::::::::::::::::::::::Creacion del socket:::::::::::::::::::::::::::::::::::::::
    if ((socketCliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Error de creación de socket \n"); 
        return -1; 
    } 
    sockServ= socketCliente;

    direccion.sin_family = AF_INET; 
    direccion.sin_port = htons(PORT); 
       
    // Convierta direcciones IPv4 e IPv6 de texto a formato binario 
    if(inet_pton(AF_INET, "127.0.0.1", &direccion.sin_addr)<=0)  
    { 
        printf("\nDirección no válida / Dirección no admitida\n"); 
        return -1; 
    } 

    if (connect(socketCliente, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) 
    { 
        printf("\nLa conexión falló \n"); 
        return -1; 
    } 

    pthread_t jugador;
    pthread_t oponente;


    int n = 0;
    char buffer[256];
    char* pbuffer = buffer;
    if ((n = recv(socketCliente, pbuffer, 256, 0)) > 0) {
        buffer[256] = '\0';
    }
    if (buffer[0]=='1'){
        
        for (int  i = 0; i < 2; i++)
        {
            if ((n = recv(socketCliente, pbuffer, 256, 0)) > 0) {
                buffer[256] = '\0';
                printf("Servidor: '%s'\n", buffer);
            }
        }
        
        usleep(1000000);
        if(pthread_create(&jugador,NULL, &mov_1 ,NULL)) {
            fprintf(stderr, "Error creating thread\n");
        }
        if(pthread_create(&oponente,NULL, &recv_2  ,NULL)) {
            fprintf(stderr, "Error creating thread\n");
        }
    }
    else{
        if(buffer[0]=='2'){
            if ((n = recv(socketCliente, pbuffer, 256, 0)) > 0) {
                buffer[256] = '\0';
                printf("\tServidor: '%s'\n", buffer);
            }
        }
        
        usleep(1000000);
        if(pthread_create(&jugador,NULL, &mov_2 ,NULL)) {
            fprintf(stderr, "Error creating thread\n");
        }
        if(pthread_create(&oponente,NULL, &recv_1  ,NULL)) {
        fprintf(stderr, "Error creating thread\n");
        }
    }
    PELOTA p(39,11,1,1);
    campo();

    
    usleep(100);
    while(fin<3){

        p.mover(a,b);
        fflush(NULL);
        usleep(100000);
    }
    
    if(pthread_join(jugador,NULL)){
        fprintf(stderr, "Error joining thread\n");
    }

    if(pthread_join(oponente,NULL)) {

        fprintf(stderr, "Error joining thread\n");
    }
    
    usleep(100000);
    return 0; 
} 


