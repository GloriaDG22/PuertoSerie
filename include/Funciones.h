//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "PuertoSerie.h"

#define NOMFICHEROF5 "log.txt"


class Funciones
{
    private:
        static Funciones* obj;
        FILE *fSal;
        bool escribir;
        Funciones();

        static void createInstance();

    public:
        static Funciones* getInstance();
        void abrirFlujo();
        void cerrarFlujo();
        void setEscribir(bool _escribir);
        void escribirCadena(string cadena);
        void escribirCaracter(char caracter);
        bool comprobarESC(bool &teclaESC);
};

#endif // FUNCIONES_H
