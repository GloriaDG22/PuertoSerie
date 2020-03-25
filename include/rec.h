#ifndef REC_H
#define REC_H

//============================================================================
// ------------ Gloria M�ndez S�nchez y Gloria D�az Gonz�lez------------------
//============================================================================
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "PuertoSerie.h"
#include "Trama.h"
using namespace std;

class rec
{
    private:
        bool esFichero;
        bool finFichero;
        static rec* obj;
        rec ();

        static void createInstance();

    public:
        static rec* getInstance();
        int calcular();
};

#endif // REC_H
