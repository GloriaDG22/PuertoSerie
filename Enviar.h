#ifndef ENVIAR_H
#define ENVIAR_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "PuertoSerie.h"

const int MAX = 10;

class Enviar
{
    private:
        char caracter;
        char cadena[MAX+2];
        int cont;

    public:
        Enviar();
        virtual ~Enviar();

        void enviarCaracter(HANDLE PuertoCOM, char carE);

        void enviarCadena(char carE);

        void addChar(char carE);

        int getCont();

        void setCont(int valor);

        char* getCadena();
};

#endif // ENVIAR_H
