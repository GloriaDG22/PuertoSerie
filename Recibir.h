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
        void recibir(char carR, int &campoT, Trama &aux);
};

#endif // RECIBIR_H
