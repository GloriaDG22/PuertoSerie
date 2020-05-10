//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#ifndef TRAMA_H
#define TRAMA_H
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "PuertoSerie.h"
#include "Funciones.h"


class Trama{
    private:
        //Carácter de sincronización (22)
        unsigned char sincr;
        //Carácter de dirección ('T' o 'R')
        unsigned char dir;
        //Carácter de control (ENQ, EOT, ACK, NACK)
        unsigned char control;
        //Carácter de número de práctica ('0' o '1')
        unsigned char numTrama;
        //Carácter de longitud del campo dato
        unsigned char longit;
        //Cadena de texto a enviar/recibir
        char Datos [255];
        //Carácter BCE (convalores entre 1 y 254)
        unsigned char BCE;
        //Instancia de la clase Funciones
        Funciones* fTrama;

    public:
        //constructor
        Trama();
        ///Setters
        /*
        * Cambia el valor de la variable sincr
        *   \param Çsincr Nuevo valor que va a guardarse en el atributo sincr de la clase
        */
        void setSincr (unsigned char Csincr);
        /*
        * Cambia el valor de la variable dir
        *   \param Cdir Nuevo valor que va a guardarse en el atributo dir de la clase
        */
        void setDir (unsigned char Cdir);
        /*
        * Cambia el valor de la variable sincr
        *   \param Ccontrol Nuevo valor que va a guardarse en el atributo control de la clase
        */
        void setControl (unsigned char Ccontrol);
        /*
        * Cambia el valor de la variable numTrama
        *   \param CnumTrama Nuevo valor que va a guardarse en el atributo sincr de la clase
        */
        void setNumTrama (unsigned char CnumTrama);
        /*
        * Cambia el valor de la variable longit
        *   \param Clongit Nuevo valor que va a guardarse en el atributo longit de la clase
        */
        void setLong (unsigned char Clongit);
        /*
        * Cambia el valor de la variable Datos
        *   \param cadena Nuevo valor que va a guardarse en el atributo Datos de la clase
        */
        void setDatos (char cadena []);
        /*
        * Cambia el valor de la variable BCE
        *   \param Cbce Nuevo valor que va a guardarse en el atributo BCE de la clase
        */
        void setBCE (unsigned char Cbce);
        /*
        * Cambia el valor de todos los atributos de la clase
        *   \param Csincr Nuevo valor que va a guardarse en el atributo sincr de la clase
        *   \param Cdir Nuevo valor que va a guardarse en el atributo dir de la clase
        *   \param Ccontrol Nuevo valor que va a guardarse en el atributo control de la clase
        *   \param CnumTrama Nuevo valor que va a guardarse en el atributo numTrama de la clase
        *   \param Clongit Nuevo valor que va a guardarse en el atributo longit de la clase
        *   \param Cdatos Nuevo valor que va a guardarse en el atributo Datos de la clase
        *   \param Cbce Nuevo valor que va a guardarse en el atributo BCE de la clase
        */
        void setAll (unsigned char Csincr, unsigned char Cdir, unsigned char Ccontrol,
              unsigned char CnumTrama, unsigned char Clongit, char Cdatos[], unsigned char Cbce);
        ///Getters
        /*
        * \return sincr Devuelve el valor del atributo sincr de la clase
        */
        unsigned char getSincr();
        /*
        * \return dir Devuelve el valor del atributo dir de la clase
        */
        unsigned char getDir();
        /*
        * \return control Devuelve el valor del atributo control de la clase
        */
        unsigned char getControl();
        /*
        * \return numTrama Devuelve el valor del atributo numTrama de la clase
        */
        unsigned char getNumTrama();
        /*
        * \return longit Devuelve el valor del atributo longit de la clase
        */
        unsigned char getLong();
        /*
        * \return Datos Devuelve el valor del atributo Datos de la clase
        */
        char* getDatos();
        /*
        * \return BCE Devuelve el valor del atributo BCE de la clase
        */
        unsigned char getBCE();

        /*
        * Muestra por pantalla el tipo de trama de control correspondiente
        */
        void imprimirTipoTrama();

        /*
        * Hace la operación lógica XOR a todos los caracteres del campo Datos de dos en dos
        * y lo devuelve
        *   \return total Devuelve el resultado de hacer la XOR a todos los caracteres del
        *   campo Datos
        */
        unsigned char calcularBce();

        /*
        * Invoca al metodo escribirCadena de la clase Funciones pasandole por parámetro el
        * campo Datos de la clase para que lo escriba por pantalla y, si procede, lo escribe
        * en el fichero
        */
        void mostrarTrama ();

};

#endif // TRAMA_H
