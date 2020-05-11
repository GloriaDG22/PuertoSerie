//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include "PuertoSerie.h"


#define PROTMAESTRO "Prolog-m.txt"
#define PROTESCLAVO "Prolog-e.txt"

class Protocolo
{
    private:
        //guarda true si se estamos en la estacion maestra, false si es estamos en la esclava
        bool esMaestro;
        //varialbe que almacena el tipo de operación: R si seleccion, T si sondeo
        char tipoOper;
        //es false mientras el maestro no quiera cerrar la comunicacion
        bool cerrar;
        //0 o 1 ciclicamente, se reinicia en cada nueva fase
        char numTrama;
        //fichero en el que se volcara todo lo relacionado con el protocolo
        FILE *ficheroProt;
        //variable que se pone a false en caso de que la trama que recibamos no es la que esperamos
        bool tCorrecta;
        //guarda true si estamos en la estación que ha pulsado F6
        bool estacionQueInicia;
        //se pone a true desde que empieza hasta que acaba el protocolo, en cualquier
        //otro caso, guarda false
        bool esProt;
        //almacena el color con el que hay que escribir en la pantalla
        int color;
        //es true si nos encontramos al final de la fase de transferencia
        bool finTransferencia;
        //se pone a true si la estación que envía el fichero no es capaz de abrirlo, false en caso contrario
        bool errorFichero;

        ///variable y métodos para el Singleton
        static Protocolo* obj;
        Protocolo();
        static void createInstance();

    public:
        static Protocolo* getInstance();

        /*
        * numTrama pasa a valer 0
        */
        void reiniciarNumTrama();
        /*
        * Cambiamos numTrama:
        *   Si era 0 ahora es 1
        *   Si era 1 ahora es 0
        */
        void cambiarNumTrama();

        /*
        * Abre el flujo de escritura:
        *   Si estamos en la estación maestra abrimos el fichero Prolog-m
        *   Si estamos en la estación esclava abrimos el fichero Prolog-e
        */
        void abrirFichero();

        /*
        * Cerramos el flujo de escritura
        */
        void cerrarFichero();

        /*
        * Se escribe una cadena por pantalla y en el fichero del Protocolo
        *   \param cadena Es la cadena que vamos a escribir
        */
        void escribirCadena(string cadena);

        /*
        * Se escribe un entero por pantalla y en el fichero del Protocolo
        *   \param numero Es el entero que vamos a escribir
        */
        void escribirEntero(int numero);

        /*
        * Se escribe un caracter por pantalla y en el fichero del Protocolo
        *   \param car Es el caracter que vamos a escribir
        */
        void escribirCaracter (char car);

        /*
        * Inicia la estación Maestra: pregunta que operación quiere realizar, y le envía
        * un mensaje al esclavo para informarlo
        *   \param PuertoCOM Puerto por el que recibimos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void iniciarProtMaestro(HANDLE &PuertoCOM, HANDLE &Pantalla);

        /*
        * Inicia la estación Esclava: se queda esperando al carácter que el maestro tiene
        * que enviarle
        *   \param PuertoCOM Puerto por el que recibimos
        *   \param Pantalla Se utiliza para cambiar el color de texto y fondo de la consola
        */
        void iniciarProtEsclavo(HANDLE &PuertoCOM, HANDLE &Pantalla);

        ///GETTERS
        /*
        *   \return tipoOper Devuelve el valor del atributo tipoOper de la clase
        */
        unsigned char getTipoOper();
        /*
        *   \return esMaestro Devuelve el valor del atributo esMaestro de la clase
        */
        bool getEsMaestro();
        /*
        *   \return cerrar Devuelve el valor del atributo cerrar de la clase
        */
        bool getCerrar();
        /*
        *   \return tCorrecta Devuelve el valor del atributo tCorrecta de la clase
        */
        bool getTCorrecta();
        /*
        *    \return numTrama Devuelve el valor del atributo numTrama de la clase
        */
        unsigned char getNumTrama();
        /*
        *   \return esProt Devuelve el valor del atributo esProt de la clase
        */
        bool getEsProt ();
        /*
        * Devuelve una cadena con el tipo de trama que corresponde al control que
        * entra por parámetros
        *   \param control Es el control del que queremos averiguar el tipo de trama
        *   \return tipoTrama Devuelve
        */
        string getTipoTrama(unsigned char control);
        /*
        *   \return color Devuelve el valor del atributo color de la clase
        */
        int getColor();
        /*
        * \return finTransferencia Devuelve el valor del atributo finTransferencia de la clase
        */
        bool getFinTransferencia();
        /*
        * \return errorFichero Devuelve el valor del atributo errorFichero de la clase
        */
        bool getErrorFichero();

        ///SETTERS
        /*
        * Cambia el valor de la variable cerrar
        *   \param _cerrar Nuevo valor que va a guardarse en el atributo cerrar de la clase
        */
        void setCerrar(bool _cerrar);
        /*
        * Cambia el valor de la variable tCorrecta
        *   \param _tCorrecta Nuevo valor que va a guardarse en el atributo tCorrecta de la clase
        */
        void setTCorrecta (bool _tCorrecta);
        /*
        * Cambia el valor de la variable estacionQueInicia
        *   \param _estacion Nuevo valor que va a guardarse en el atributo estacionQueInicia de la clase
        */
        void setEstacionQueInicia (bool _estacion);
        /*
        * Cambia el valor de la variable esProt
        *   \param _esProt Nuevo valor que va a guardarse en el atributo esProt de la clase
        */
        void setEsProt (bool _esProt);
        /*
        * Cambia el valor de la variable color
        *   \param _color Nuevo valor que va a guardarse en el atributo color de la clase
        */
        void setColor (int _color);
        /*
        * Cambia el valor de la variable finTransferencia
        *   \param finT Nuevo valor que va a guardarse en el atributo finTransferencia de la clase
        */
        void setFinTransferencia(bool finT);
        /*
        * Cambia el valor de la variable errorFichero
        *   \param _errorFichero Nuevo valor que va a guardarse en el atributo errorFichero de la clase
        */
        void setErrorFichero(bool _errorFichero);

        /*
        * Dados unos parámetros por pantalla, los imprime teniendo en cuenta si es una trama de datos (control=2)
        * o de control (control!=2) para mostrar el bce o no
        *   \param control Atributo control de la trama que queremos imprimir
        *   \param bce Atributo bce de la trama que queremos imprimir: si es de control contendrá un valor basura
        *   \param dir Atributo dir de la trama que queremos imprimir
        *   \param nTrama Atributo numTrama de la trama que queremos imprimir
        *   \param aux Contiene E si la trama a imprimir ha sido Enviada o R si ha sido Recibida
        */
        void imprimirTrama(unsigned char control, unsigned char bce, unsigned char dir, unsigned char nTrama, string aux);

        /*
        * Metodo para inicializar las variables necesarias para que el protocolo pueda funcionar correctamente
        */
        void iniciarVariables();
};

#endif // PROTOCOLO_H
