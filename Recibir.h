//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef RECIBIR_H
#define RECIBIR_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "PuertoSerie.h"
#include "Trama.h"

class Recibir
{
    private:

    public:
        /** Crea la trama con los datos que le llegan e imprime en pantalla el tipo de trama qu eha recibido.
        *   \param carR Caracter recibido
        *   \param campoT Controla por que caracter de la trama ha leido
        *   \param aux Trama que va construyendo como parámetro de entrada salida
        *   \param PuertoCOM Puerto por el que recibimos
        */
        void recibir(char carR, int &campoT, Trama &aux, HANDLE PuertoCOM);
};

#endif // RECIBIR_H
