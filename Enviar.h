#ifndef ENVIAR_H
#define ENVIAR_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "PuertoSerie.h"
#include "Trama.h"

const int MAX = 800;

//Campo 1: Sincronismo (SYN): en codigo ASCII=22
//Campo 2: direcci�n ('T' o 'R'): por ahora solo vamos a usar el valor 'T'
//Campo 3: control (ENQ, EOT, ACK, NACK): en codigo ASCII: ENQ=05, EOT=05, ACK=06, NACK=21
//Campo 4: n�mero de trama ('0' � '1'): siempre como car�cter (entre ' '): por ahora siempre vamos a usar el '0'

class Enviar
{
    private:
        char caracter;
        char cadena[MAX+2];
        int cont;

    public:
        Enviar();
        virtual ~Enviar();

        void comprobarTeclaFuncion (char carE, HANDLE PuertoCOM);

        void enviarCaracter(HANDLE PuertoCOM, char carE);

        void enviarCadena(char carE);

        void crearTrama (HANDLE PuertoCOM);

        void addChar(char carE);

        int getCont();

        void setCont(int valor);

        char* getCadena();

        void enviarTrama(Trama t, HANDLE PuertoCOM);
};

#endif // ENVIAR_H
