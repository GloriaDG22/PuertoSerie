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
        //Flujo que se usa para abrir el fichero de escriruta en F5
        FILE *fSal;
        //variable que se pone a true si la tecla F5 esta activada y false en caso contrario
        bool escribir;

        ///variable y métodos para el Singleton
        static Funciones* obj;
        Funciones();
        static void createInstance();

    public:
        static Funciones* getInstance();

        /*
        * Devuelve en numTramas el numero de tramas que se tienen que enviar para una longitud
        * de cadena igual a cont
        *   \param cont Numero de carácteres que el usuario quiere enviar
        *   \param numTramas Parámetro de entrada salida que guarda el número de tramas
        */
        void dividirCadena(int cont, int &numTramas);

        /*
        * Copia la cadena fuene en la destino desde la posición deseada hasta el tamaño indicado
        *   \param cadenaFuente Cadena a copiar
        *   \param cadenaDestino Cadena de destino
        *   \param offset Desplazamiento para empezar a copiar
        *   \param longit Número de carácteres que se quieren copia
        */
        void copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit);

        /*
        * Abrimos el flujo del fichero fSal
        */
        void abrirFlujo();

        /*
        * Cerramos el flujo del fichero fSal
        */
        void cerrarFlujo();

        /*
        * Cambiamos el valor de la variable escribir
        *   \param _escribir nuevo valor de la variable escribir
        */
        void setEscribir(bool _escribir);

        /*
        * Escribe una cadena por pantalla y si el valor de la variable escribir es true
        * escribe esa misma cadena en el fichero log.txt
        *   \param cadena Lo que vamos a escribir por pantalla, y en el fichero si procede
        */
        void escribirCadena(string cadena);

        /*
        * Si la variable escribir es true escribre una cadena en el fichero log.txt
        *   \param cadena Lo que queremos escribir en el fichero
        */
        void escribirCadenaSoloEnFichero(string cadena);

        /*
        * Escribe un caracter por pantalla y si el valor de la variable escribir es true
        * escribe ese mismo caracter en el fichero log.txt
        *   \param caracter Lo que vamos a escribir por pantalla, y en el fichero si procede
        */
        void escribirCaracter(char caracter);

        /*
        * Comprueba si hay pulsación de tecla y, en caso de haberla comprueba que tecla es y
        * devuelve un valor en función de la tecla que sea
        *   \param tipoTecla Devolverá 1 si la tecla leida es la tecla de escape, 2 si es F7 y
        *   3 en cualquier otro caso
        */
        int comprobarTecla();
};

#endif // FUNCIONES_H
