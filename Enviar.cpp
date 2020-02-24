#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
}

Enviar::~Enviar(){
}

int Enviar::getCont(){
    return cont;
}

void Enviar::setCont(int valor){
    cont=valor;
}

char* Enviar::getCadena(){
    return cadena;
}

void Enviar::comprobarTeclaFuncion(char carE, HANDLE PuertoCOM){
    switch (carE){
        case 59:
            printf("\n");
            addChar('\n');
            addChar('\0');
            EnviarCadena(PuertoCOM, cadena, cont);
            cont=0;
            break;
        case 60:
            crearTrama(PuertoCOM);
    }
}

void Enviar::enviarCaracter(HANDLE PuertoCOM, char carE){
    printf("%c", carE);   //Envío
    EnviarCaracter(PuertoCOM,caracter);
}

void Enviar::enviarCadena (char carE){
    switch (carE){
    case 8:
        if(cadena[cont-1]!='\n'){
            //printf("\b \b");
            printf("%c", carE);
            printf(" ");
            printf("%c", carE);
            cont--;
        }
        break;
    case 13:
        if(cont < MAX){
            printf("\n");
            cadena[cont] = '\n';
            cont++;
        }
        break;
    default:
        if(cont < MAX){
            printf("%c", carE);
            cadena[cont]=carE;
            cont++;
        }
        break;
    }
}

void Enviar::addChar(char carE){
        cadena[cont]=carE;
        cont++;
}

void Enviar::crearTrama(HANDLE PuertoCOM){
    printf ("\nElija el tipo de trama que quiere enviar \n 1: Trama ENQ \n 2: Trama EOT \n 3: Trama ACK \n 4: Trama NACK \n");
    int opcionT;
    unsigned char control;
    bool valorCorrecto=false;
    bool envio=true;
    while (!valorCorrecto){
        opcionT = getch();
        switch (opcionT){
            case '1':
                control = 05;
                valorCorrecto = true;
                break;
            case '2':
                control = 04;
                valorCorrecto = true;
                break;
            case '3':
                control = 06;
                valorCorrecto = true;
                break;
            case '4':
                control = 21;
                valorCorrecto = true;
                break;
            case 27:
                printf ("Se ha cancelado el envio de la trama \n");
                valorCorrecto = true;
                envio = false;
                break;
            default:
                 printf("Ha introducido un valor erroneo, intentelo de nuevo \n");
                break;
        }
    }
    if (envio==true){
        Trama t=Trama();
        t.setSincr(22);
        t.setDir('T');
        t.setControl(control);
        t.setNumTrama('0');
        enviarTrama(t, PuertoCOM);
    }
}

void Enviar::enviarTrama(Trama t, HANDLE PuertoCOM){
    EnviarCaracter(PuertoCOM, t.getSincr());
    EnviarCaracter(PuertoCOM, t.getDir());
    EnviarCaracter(PuertoCOM, t.getControl());
    EnviarCaracter(PuertoCOM, t.getNumTrama());
}

