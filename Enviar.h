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

#define FICHERO "fichero-e.txt" //Nombre del fichero que se envia en F3
#define FICHEROLECTURA "EProtoc.txt" //Nombre del fichero que se envia en F6

#define NUM_ESC 1
#define NUM_F7 2
#define SIN_NUM 3

///COLORES PROTOCOLO
#define CESTABLECIMIENTO 9 //AZUL ELECTRICO
#define CTRANSFERENCIA1  12 //ROJO
#define CTRANSFERENCIA2  2 //AZUL VERDOSO
#define CTRANSFERENCIA3  8 //GRIS OSCURO
#define CCIERRE 11 //CIAN

const int MAX = 800; //máximo de caracteres del texto a enviar
const int LINCABECERA = 3; //numero de lineas de la cabecera del fichero

class Enviar
{
    private:
        //caracter que se envía
        char caracter;
        //cadena de texto que se escribe en el terminal
        char cadena[MAX+2];
        //número de carácter que hay en la cadena
        int cont;
        //Envio: letra azul verdoso (3) y fondo negro (0)
        int colorEnvio;
        //guarda los autores leidos del fichero
        char autores[50];
        //variable en la que se guarda lo que se lee del fichero
        char texto[255];
        // variable que indica si se ha pulsado F7
        bool errorProt;
        //se pone a true si se recibe una EOT durante la fase de
        //transferencia (cuando el que envia no ha podido abrir el fichero)
        bool errorFichero;
        //Trama que se va a enviar
        Trama tEnvio;
        //Instancia de la clase Recibir para no excluir la recepción
        Recibir* recibo;
        //Instancia de la clase Funciones
        Funciones* fEnvio;
        //Instancia de la clase Protocolo
        Protocolo* pEnvio;

    public:
        //constructor
        Enviar();

        ///GETTERS
        /*
        * Devuelve el valor del contador
        */
        int getCont();

        ///SETTERS
        /*
        * Cambia el valor del contador
        *   \param valor Nuevo valor de contador
        */
        void setCont(int valor);

        /*
        * Añade un valor a la cadena a enviar
        *   \param carE Caracter que queremos añadir
        */
        void addChar(char carE);

        /*
        * Comprueba qué tecla hemos accionado para ver como actuar
        *   \param carE Indica la tecla que se ha pulsado
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void comprobarTeclaFuncion (char carE, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Construye la cadena que vamos a enviar
        *   \param carE Caracter que vamos a añadir a la cadena
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note Si es tecla normal guarda en la cadena por defecto y si es borrar o enter
        *       entra en sus respectivos casos
        */
        void enviarCadena(char carE, HANDLE &Pantalla);

        /*
        * Crea la trama de control en función de la tecla que pulsa el usuario y la envía
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note tenemos en cuenta la pulsación de la tecla ESC
        */
        void crearTramaControl (HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Divide el texto que queremos enviar en distintas tramas de datos y las envía
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void crearTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Envía caracter a caracter diferenciando si la trama es de control o de datos
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note la trama que se envía es la variable de la clase tEnvio
        *   \note no excluímos envío y recepción
        */
        void enviarTrama(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Lee el fichero dividiendolo en tramas, y las va enviando
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note tenemos en cuenta la pulsación de la tecla ESC
        */
        void enviarFichero (HANDLE &PuertoCOM, HANDLE &Pantalla);

///********************************MÉTODOS DEL PROTOCOLO**************************************
        /*
        * Comprueba la variable tipoOper y dependiendo del valor que tenga, llama a la operación
        * sondeo o la operación selección
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void llamaOperacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Dependiendo de si la estación es maestro o es esclavo, hace las llamadas necesarias
        * a los métodos de envío y recepción necesarios en cada caso para la operación de sondeo
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void sondeo(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Dependiendo de si la estación es maestro o es esclavo, hace las llamadas necesarias
        * a los métodos de envío y recepción necesarios en cada caso para la operación de selección
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void seleccion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Corresponde con la fase de establecimiento: la estación es maestro manda una ENQ para
        * entablar conversación con el esclavo y espera la respuesta de éste, por el contrario,
        * si es esclavo, espera la trama ENQ que el maestro tiene que mandarle y le envía una
        * trama de aceptación si esta ha llegado correctamente
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void faseEstablecimiento(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Corresponde con la fase de transferencia de la estación que envía: lee el fichero dividiendolo
        * en tramas que va enviando
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note la fase acaba cuando se lee el fichero completo o si no puede abrir el fichero
        *   \note tenemos en cuenta la pulsación de la tecla ESC
        */
        void faseTransferenciaEnvio(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Corresponde con la fase de transferencia de la estación que recibe: mientras la variable
        * finTransferencia este a false se queda recibiendo tramas y, si estas son correctas, envía
        * la trama de confirmación correspondiente
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \note si recibiendo una trama de datos recibe una EOT se sale ya que significaría que el
        *   emisor no ha podido abrir el fichero
        */
        void faseTransferenciaRecibo(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Corresponde con la fase de cierre: se envía una EOT para cerrar la comunicación y se espera
        * una respuesta de la otra estacion
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void faseCierre(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Pregunta al usuario si quiere cerrar la comunicación con el esclavo: si no se acepta se envía
        * una NACK y se queda esperando una EOT de nuevo, en el momento en el que se acepta se envía una ACK
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void aceptarCierreComunicacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Crea una trama con las variables tipoOper y NumTrama de la clase Protocolo y el control que
        * se indique y la envía
        *   \param control Campo control con el que se va a crear la trama
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void crearTramaProt(unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Invoca al método RecibirCaracter hasta que éste le devuelva un control distinto de 0, lo que significa
        * que se ha recibido una trama completa y, si este control es el que se esperaba, pone la variable
        * tCorrecta a true
        *   \param control Tipo de trama que se espera recibir
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void esperarRespuesta (unsigned char control, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Si la variable tCorrecta es true envía una trama ACK, si es false envía una NACK
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void enviarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Invoca al método RecibirCaracter hasta que éste le devuelva un control distinto de 0, lo que significa
        * que se ha recibido una trama completa y, si este control es 21 (Trama NACK) pone la variable esTramaACK
        * a false, en caso contrario, seguirá a true
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        *   \return esTramaACK es true si se recibe una trama ACK y false si se recibe NACK
        */
        bool esperarTramaAceptacion(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Invoca al método RecibirCaracter hasta que éste le devuelva un control distinto de 0, lo que significa
        * que se ha recibido una trama completa y, si este control es 4 (Trama EOT), significa que el emisor no ha podido
        * abrir el fichero y ha invocado el cierre de la comunicación antes de tiempo, por lo que pone las fariables
        * errorFichero, finTransferencia y tCorrecta a true
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void recibirTramaDatos(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * antes de dividir el fichero en tramas, comprueba si se ha pulsado la tecla F7 y, si es así, suma uno al contError,
        * después lee el fichero y lo guarda en una trama. En caso de que errorProt este a true, introduce el caracter
        * 'ç' en la primera posicion del texto. Por útlimo envía la trama
        *   \param fEnt Flujo de lectura con el que se ha abierto el fichero
        *   \param i Sirve para saber cuando hemos llegado al final de la cabecera
        *   \param linFichero Sirve para indicar si estamos leyendo la cabecera (linFichero=1) o el cuerpo (linFichero=2)
        *   \param cont Lleva la cuenta del numero de caracteres totales que hay en el fichero para luego enviarlo
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        bool trocearFicheroProt(ifstream &fEnt, int &i, int &linFichero, int &cont, HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Llama al método imprimirTrama de la clase protocolo con los valores de la variable trama de la clase Enviar
        */
        void imprimirTrama();

        /*
        * Se usa cuando se ha introducido un error en la trama enviada con anterioridad, por lo que vuelve a guardar el campo
        * datos (esta vez sin errores) en la Trama de la clase y la envia
        *   \param PuertoCOM Puerto por el que enviamos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void retransmision(HANDLE &PuertoCOM, HANDLE &Pantalla);

        //destructor
        virtual ~Enviar();
};

#endif // ENVIAR_H
