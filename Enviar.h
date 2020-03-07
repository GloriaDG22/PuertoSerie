#ifndef ENVIAR_H
#define ENVIAR_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "PuertoSerie.h"
#include "Trama.h"
#include "Recibir.h"

const int MAX = 800;

//Campo 1: Sincronismo (SYN): en codigo ASCII=22
//Campo 2: dirección ('T' o 'R'): por ahora solo vamos a usar el valor 'T'
//Campo 3: control (ENQ, EOT, ACK, NACK): en codigo ASCII: ENQ=05, EOT=05, ACK=06, NACK=21
//Campo 4: número de trama ('0' ó '1'): siempre como carácter (entre ' '): por ahora siempre vamos a usar el '0'

class Enviar
{
    private:
        char caracter;
        char cadena[MAX+2];
        int cont;
        Recibir recibo;

    public:
        Enviar();
        virtual ~Enviar();

        void comprobarTeclaFuncion (char carE, HANDLE PuertoCOM);

        void enviarCaracter(HANDLE PuertoCOM, char carE);

        void enviarCadena(char carE);

        void crearTramaControl (HANDLE PuertoCOM);

        void addChar(char carE);

        int getCont();

        void setCont(int valor);

        void crearTramaDatos(HANDLE PuertoCOM);

        void enviarTrama(Trama t, HANDLE PuertoCOM);

        void dividirCadena(int cont, int &numTramas);

        void copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit);
};

#endif // ENVIAR_H
