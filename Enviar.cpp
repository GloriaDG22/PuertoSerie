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

void Enviar::enviarCaracter(HANDLE PuertoCOM, char carE){
    caracter=carE;
    printf("%c", caracter);   //Envío
    EnviarCaracter(PuertoCOM,caracter);
}

void Enviar::enviarCadena (char carE){
    switch (carE){
    case 8:
        if(cadena[cont]!='\n'){
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

