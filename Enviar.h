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
#define FICHEROLECTURA "EProtoc.txt"

///COLORES PROTOCOLO
#define CESTABLECIMIENTO 9 //AZUL ELECTRICO
#define CTRANSFERENCIA1  12 //ROJO
#define CTRANSFERENCIA2  2 //AZUL VERDOSO
#define CTRANSFERENCIA3  8 //GRIS OSCURO
#define CCIERRE 11 //CIAN

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
        ///
        char autores[50];
        ///
        char texto[255];
        ///
        bool errorProt;
        ///
        int contError;
        ///se pone a true si se recibe una EOT durante la fase de transferencia (cuando el que envia no ha podido abrir el fichero)
        bool errorFichero;
        ///Trama que se va a enviar
        Trama tEnvio;
        ///Clase recibir para no excluir la recepción
        Recibir* recibo;
        ///
        Funciones* fEnvio;

        Protocolo* pEnvio;

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
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void comprobarTeclaFuncion (char carE, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Construye la cadena que vamos a enviar
        *   \param carE Caracter que vamos a guardar en la cadena
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note Si es tecla normal guarda en la cadena por defecto y si es borrar o enter entra en casos especiales
        */
        void enviarCadena(char carE, HANDLE &Pantalla);

        /** Crea la trama de control en función de lo deseado por el usuario y la envía
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void crearTramaControl (HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Divide el texto que queremos enviar en distintas tramas de datos
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note no excluímos la recepcion de mensajes
        */
        void crearTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Envía caracter a caracter diferenciando si es de control o de datos
        *   \param t Trama a enviar
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void enviarTrama(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /** Lee las líneas del fichero y las envía
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void enviarFichero (HANDLE &PuertoCOM, HANDLE &Pantalla);

        void llamaOperacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        void sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla);

        void seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void faseTransferenciaEnvio(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void esperarRespuesta (unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void enviarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void esperarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void crearTramaProt(unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /**
        */
        void recibirTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla);

        void imprimirTrama();

        void trocearFicheroProt(ifstream &fEnt, int &i, int &linFichero, int &cont, HANDLE &PuertoCOM, HANDLE &Pantalla);
};

#endif // ENVIAR_H
