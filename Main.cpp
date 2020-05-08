//============================================================================
// ----------- PR�CTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2019/20 --------------------------------
// ----------------------------- MAIN.CPP ---------------------------------
// ------------ Gloria M�ndez S�nchez y Gloria D�az Gonz�lez------------------
//============================================================================
#include "Enviar.h"
#include "Recibir.h"

using namespace std;

HANDLE PuertoCOM;
HANDLE Pantalla;

int main()
{
    char carE, carR = 0;
    Pantalla = GetStdHandle(STD_OUTPUT_HANDLE);

    Enviar envio=Enviar();
    Recibir* recibo=recibo->getInstance();


    //Encabezado
    printf("============================================================================\n");
    printf("----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------\n");
    printf("---------------------------- CURSO 2019/20 ---------------------------------\n");
    printf("----------------------------- SESION1.CPP ----------------------------------\n");
    printf("============================================================================\n\n");

    //Abrimos el puerto. Para ello necesitamos indicar los siguientes par�metros:
    // - Nombre del puerto a abrir: ("COM1", "COM2", "COM3", ...).
    // - Velocidad: (1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200).
    // - N�mero de bits en cada byte enviado o recibido: (4, 5, 6, 7, 8).
    // - Paridad: (0=sin paridad, 1=impar, 2=par, 3=marca, 4=espacio).
    // - Bits de stop: (0=1 bit, 1=1.5 bits, 2=2 bits).

    char PSerie[5] = {'C', 'O', 'M', ' ', '\0'};
    char opcion;
    bool errorOpcion=true;

    int velocidad;

    printf("Seleccione que puerto quiere abrir: \n 1. COM1 \n 2. COM2 \n 3. COM3 \n 4. COM4 \n");

    while(errorOpcion){
        opcion = getch();
        printf ("Has seleccionado la tecla %c \n", opcion);

        switch(opcion){
        case '1':
            PSerie[3] = '1';
            errorOpcion = false;
            break;
        case '2':
            PSerie[3] = '2';
            errorOpcion = false;
            break;
        case '3':
            PSerie[3] = '3';
            errorOpcion = false;
            break;
        case '4':
            PSerie[3] = '4';
            errorOpcion = false;
            break;
        default:
            printf("Introduzca un valor correcto.\n");
            break;
        }
    }


   printf("Seleccione la velocidad de transmision: \n 1. 1200 \n 2. 2400 \n 3. 4800 \n 4. 9600 \n 5. 19200 \n");
   errorOpcion = true;

    while(errorOpcion){
        opcion = getch();
        printf ("Has seleccionado la tecla %c \n", opcion);
        switch(opcion){
        case '1':
            velocidad = 1200;
            errorOpcion = false;
            break;
        case '2':
            velocidad = 2400;
            errorOpcion = false;
            break;
        case '3':
            velocidad = 4800;
            errorOpcion = false;
            break;
        case '4':
            velocidad = 9600;
            errorOpcion = false;
            break;
        case '5':
            velocidad = 19200;
            errorOpcion = false;
            break;
        default:
            printf("Introduzca un valor correcto.\n");
            break;
        }
   }

    PuertoCOM = AbrirPuerto(PSerie, velocidad, 8, 0, 0);
    ///Abrimos el puerto COM1 (en la sala siempre abrimos el COM1)
    if(PuertoCOM == NULL)
    {
        printf("Error al abrir el puerto %s con velocidad %d \n", PSerie, velocidad);
        getch();
        exit(1);
    }
    else
        printf("Puerto %s abierto correctamente con velocidad %d \n", PSerie, velocidad);

    /// Lectura y escritura simult�nea de caracteres:
    while(carE != 27){

        carR = RecibirCaracter(PuertoCOM);
        recibo->recibir(carR, PuertoCOM, Pantalla);

        if((carR=='M'||carR=='E')&&recibo->getEsProt())
            envio.llamaOperacion(PuertoCOM, Pantalla);

        if (kbhit()){
            carE = getch();
            if (carE != 27){
                if(carE == '\0'){
                    carE = getch();
                    envio.comprobarTeclaFuncion(carE, PuertoCOM, Pantalla);
                }
                else{
                    envio.enviarCadena(carE, Pantalla);
                }
            }
        }
    }


// Cerramos el puerto:
    if (PuertoCOM!=NULL)
        CerrarPuerto(PuertoCOM);
    envio.~Enviar();
    recibo->~Recibir();
    if (recibo!=0)
        delete recibo;
   return 0;
}
