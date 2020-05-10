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
        //Instancia de la clase Trama
        Trama tRecibida;
        //Variable que guarda que campo de la trama vamos a recibir
        int campoT;
        //Guarda la linea autores que se lee del fichero
        char autores [50];
        //Guarda el color que se lee del fichero
        char *color;
        //Guarda los nombres de los autores que se leen del fichero
        char nomFichero [40];
        //Sirve para saber si estamos procesando la linea de los autores (lineaFichero=1),
        //el campo del color (lineaFichero=2), el campo del nombre del fichero (lineaFichero=3)
        //o el resto de lineas
        int lineaFichero;
        //color para recibir fichero
        int colorFichero;
        //color de recibo por defecto
        int colorRecibo;
        //variable que guarda true desde que se recibe el caracter '}', que indica el inicio
        //de la recepción del fichero, hasta que se recibe el caracter '}', que indica el final
        //de la recepción del fichero, y false en cualquier otro caso
        bool esFichero;
        //variable que guarda true cuando se recibe el caracter '}', que indica que ya solo queda
        //recibir la trama que guarda el numero de caracteres totales del fichero, y se pone a false
        //después de recibir esta trama
        bool finFichero;
        //Instancia de la clase Funciones
        Funciones* fRecibo;
        //Instancia de la clase Protocolo
        Protocolo* pRecibo;
        //Flujo de salida utilizado para abrir el fichero con el nombre leido del fichero y escribir en éste
        ofstream fSal;

        ///variable y métodos para el Singleton
        static Recibir* obj;
        Recibir ();
        static void createInstance();

    public:
        static Recibir* getInstance();

        /* Crea la trama con los datos que le llegan e imprime en pantalla el tipo de trama qu eha recibido.
        *   \param carR Caracter recibido
        *   \param campoT Controla por que caracter de la trama ha leido
        *   \param aux Trama que va construyendo como parámetro de entrada salida
        *   \param PuertoCOM Puerto por el que recibimos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        unsigned char recibir(char carR, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /* Se procesan las tramas de datos correspondientes a un fichero: las 3 primeras lineas (cabecera) se
        *   guardan pero no se muestran por pantalla y el cuerpo del fichero se escribe en un fichero de salida
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void procesarFichero (HANDLE Pantalla);

        /*
        *   \return devuelve la variable esProt de la clase Protocolo
        */
        bool getEsProt();

        //destructor
        virtual ~Recibir();
};

#endif // RECIBIR_H
