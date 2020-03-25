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
#include <fstream>
#include "PuertoSerie.h"
#include "Trama.h"
using namespace std;

class Recibir{

    private:
        char autores [50];
        char color [2];
        char nomFichero [40];
        int lineaFichero;
        int colorTexto;
        bool esFichero;
        bool finFichero;
        static Recibir* obj;
        Recibir ();
        ofstream fSal;

        static void createInstance();

    public:
        static Recibir* getInstance();

        /** Crea la trama con los datos que le llegan e imprime en pantalla el tipo de trama qu eha recibido.
        *   \param carR Caracter recibido
        *   \param campoT Controla por que caracter de la trama ha leido
        *   \param aux Trama que va construyendo como parámetro de entrada salida
        *   \param PuertoCOM Puerto por el que recibimos
        */
        void recibir(char carR, int &campoT, Trama &aux, HANDLE PuertoCOM, HANDLE Pantalla);

        void procesarFichero (HANDLE Pantalla, Trama t, ofstream fSal);
};

#endif // RECIBIR_H
