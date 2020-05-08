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
#include "Funciones.h"
#include "Protocolo.h"

#define CTRANSFERENCIA3  8 //GRIS OSCURO
#define CCIERRE 11  //CIAN

class Recibir{

    private:
        static Recibir* obj;
        Trama tRecibida;
        int campoT;
        char autores [50];
        char *color;
        char nomFichero [40];
        int lineaFichero;
        int colorFichero; ///color para recibir fichero
        int colorRecibo; ///color de recibo por defecto
        bool esFichero;
        bool finFichero;
        Funciones* fRecibo;
        Protocolo* pRecibo;

        ofstream fSal;

        Recibir ();

        static void createInstance();

    public:
        static Recibir* getInstance();

        /** Crea la trama con los datos que le llegan e imprime en pantalla el tipo de trama qu eha recibido.
        *   \param carR Caracter recibido
        *   \param campoT Controla por que caracter de la trama ha leido
        *   \param aux Trama que va construyendo como parámetro de entrada salida
        *   \param PuertoCOM Puerto por el que recibimos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        unsigned char recibir(char carR, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Se procesan las tramas de datos correspondientes a un fichero: las 3 primeras lineas (cabecera) se
        *   guardan pero no se muestran por pantalla y el cuerpo del fichero se escribe en un fichero de salida
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void procesarFichero (HANDLE Pantalla);

        bool getEsProt();

        virtual ~Recibir();
};

#endif // RECIBIR_H
