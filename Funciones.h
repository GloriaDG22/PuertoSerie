//============================================================================
// ------------ Gloria M�ndez S�nchez y Gloria D�az Gonz�lez------------------
//============================================================================

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "PuertoSerie.h"
#include "Protocolo.h"

#define NOMFICHEROF5 "log.txt"

//class Protocolo;
//class Funciones;

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

        /** Devuelve el numero de tramas que se van a enviar
        *   \param cont Numero de car�cteres que el usuario quiere enviar
        *   \param numTramas Par�metro de entrada salida que guarda el n�mero de tramas
        */
        void dividirCadena(int cont, int &numTramas);

        /** Copia la cadena fuene en la destino desde la posici�n deseada hasta el tama�o indicado
        *   \param cadenaFuente Cadena a copiar
        *   \param cadenaDestino Cadena de destino
        *   \param offset Desplazamiento para empezar a copiar
        *   \param longit N�mero de car�cteres que se quieren copia
        */
        void copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit);

        /** Abrimos el flujo del fichero fSal
        */
        void abrirFlujo();

        /** Cerramos el flujo del fichero fSal
        */
        void cerrarFlujo();

        /** Cambiamos el valor de la variable escribir
        *   \param _escribir nuevo valor de la variable escribir
        */
        void setEscribir(bool _escribir);

        /** Escribe una cadena por pantalla y si el valor de la variable escribir es true
        *   escribe esa misma cadena en el fichero log.txt
        *   \param cadena Lo que vamos a escribir por pantalla, y en el fichero si procede
        */
        void escribirCadena(string cadena);

        /** Escribe un caracter por pantalla y si el valor de la variable escribir es true
        *   escribe ese mismo caracter en el fichero log.txt
        *   \param caracter Lo que vamos a escribir por pantalla, y en el fichero si procede
        */
        void escribirCaracter(char caracter);

        /** Comprueba si hay pulsaci�n de tecla y, en caso de haberla, devuelve, en un par�metro
        *   de entrada salida, true si la tecla pulsada es ESC y false en caso contrario
        *   \param teclaESC Booleano en el que se devuelve true si se ha pulsado ESC y false en
        *   cualquier otro caso
        */
        bool comprobarESC(bool &teclaESC);

        void iniciarProtMaestro(HANDLE &PuertoCOM);

        void iniciarProtEsclavo(HANDLE &PuertoCOM);
};

#endif // FUNCIONES_H
