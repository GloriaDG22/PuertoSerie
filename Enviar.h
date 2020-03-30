//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef ENVIAR_H
#define ENVIAR_H
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "PuertoSerie.h"
#include "Trama.h"
#include "Recibir.h"
#include "Funciones.h"

#define FICHERO "fichero-e.txt"

const int MAX = 800;
const int LINCABECERA = 3;

//Campo 1: Sincronismo (SYN): en codigo ASCII=22
//Campo 2: dirección ('T' o 'R'): por ahora solo vamos a usar el valor 'T'
//Campo 3: control (ENQ, EOT, ACK, NACK): en codigo ASCII: ENQ=05, EOT=05, ACK=06, NACK=21
//Campo 4: número de trama ('0' ó '1'): siempre como carácter (entre ' '): por ahora siempre vamos a usar el '0'

class Enviar
{
    private:
        ///caracter que se envía
        char caracter;
        ///cadena de texto que se escribe en el terminal
        char cadena[MAX+2];
        ///número de carácter que hay en la cadena
        int cont;
        ///Envio: letra azul verdoso (3) y fondo negro (0)
        int colorEnvio;
        ///Trama que se va a enviar
        Trama tEnvio;
        ///Clase recibir para no excluir la recepción
        Recibir* recibo;
        ///
        Funciones* fEnvio;

    public:
        ///constructor
        Enviar();

        ///destructor
        virtual ~Enviar();

        ///Devuelve el valor del contador
        int getCont();

        /** Cambia el valor del contador
        *   \param valor Nuevo valor de contador
        */
        void setCont(int valor);

        /** Añade un valor a la cadena a enviar
        *   \param carE Caracter que queremos añadir
        */
        void addChar(char carE);

        /**  Comprueba qué tecla hemos accionado para ver como actuar
        *   \param carE Caracter que se pulsa
        *   \param PuertoCOM Puerto por el que enviamos
        */
        void comprobarTeclaFuncion (char carE, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Construye la cadena que vamos a enviar
        *   \param carE Caracter que vamos a guardar en la cadena
        *   \note Si es tecla normal guarda en la cadena por defecto y si es borrar o enter entra en casos especiales
        */
        void enviarCadena(char carE, HANDLE &Pantalla);

        /** Crea la trama de control en función de lo deseado por el usuario y la envía
        *   \param PuertoCOM Puerto por el que enviamos
        */
        void crearTramaControl (HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Divide el texto que queremos enviar en distintas tramas de datos
        *   \param PuertoCOM Puerto por el que enviamos
        *   \note no excluímos la recepcion de mensajes
        */
        void crearTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Envía caracter a caracter diferenciando si es de control o de datos
        *   \param t Trama a enviar
        *   \param PuertoCOM Puerto por el que enviamos
        */
        void enviarTrama(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Devuelve el numero de tramas que se van a enviar
        *   \param cont Numero de carácteres que el usuario quiere enviar
        *   \param numTramas Parámetro de entrada salida que guarda el número de tramas
        */
        void dividirCadena(int cont, int &numTramas);

        /** Copia la cadena fuene en la destino desde la posición deseada hasta el tamaño indicado
        *   \param cadenaFuente Cadena a copiar
        *   \param cadenaDestino Cadena de destino
        *   \param offset Desplazamiento para empezar a copiar
        *   \param longit Número de carácteres que se quieren copia
        */
        void copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit);

        void enviarFichero (HANDLE &PuertoCOM, HANDLE &Pantalla);

};

#endif // ENVIAR_H
