#include "Recibir.h"

void Recibir::recibir(char carR, int &campoT, Trama &aux, HANDLE PuertoCOM){
    if (carR){
        switch (campoT){
        case 1:
            if (carR==22){ //es una trama
                aux=Trama();
                campoT++;
            }
            break;
        case 2:
            campoT ++;
            break;
        case 3:
            aux.setControl(carR);
            campoT++;
            break;
        case 4:
            if (aux.getControl()!=2){
                campoT=1;
                printf ("Se ha recibido una trama de tipo ");
                aux.imprimirTipoTrama();
            }
            else
                campoT++;
            break;
        case 5:
            aux.setLong((unsigned char)carR);
            campoT++;
        case 6:
            char cadenaRecibida [254];
            RecibirCadena(PuertoCOM, cadenaRecibida, aux.getLong());
            aux.setDatos(cadenaRecibida);
            campoT++;
            break;
        case 7:
            campoT=1;
            aux.setBCE((unsigned char)carR);
            if(aux.calcularBce()==(unsigned char)carR){
                aux.mostrarTrama();
            }
            else
                printf ("Error: la cadena recibida es incorrecta \n");
        }
    }
}
