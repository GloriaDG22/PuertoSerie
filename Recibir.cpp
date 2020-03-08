//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Recibir.h"

void Recibir::recibir(char carR, int &campoT, Trama &aux, HANDLE PuertoCOM){
    if (carR){
        switch (campoT){
        case 1: //sincronización (22)
            if (carR==22){ //es una trama
                aux=Trama();
                campoT++;
            }
            break;
        case 2: //dirección ('T')
            campoT ++;
            break;
        case 3: //control ENQ-5  EOT-4  ACK-6  NACK-21 / DATOS-2
            aux.setControl(carR);
            campoT++;
            break;
        case 4: //numero de trama (0)
            if (aux.getControl()!=2){
                campoT=1;
                printf ("Se ha recibido una trama de tipo ");
                aux.imprimirTipoTrama();
            }
            else
                campoT++;
            break;
        case 5: //longitud
            aux.setLong((unsigned char)carR);
            campoT++;
        case 6: //datos
            char cadenaRecibida [254];
            RecibirCadena(PuertoCOM, cadenaRecibida, aux.getLong());
            aux.setDatos(cadenaRecibida);
            campoT++;
            break;
        case 7: //BCE
            campoT=1;
            aux.setBCE((unsigned char)carR);
            if(aux.calcularBce()==(unsigned char)carR){
                aux.mostrarTrama();
                printf("MENSAJE RECIBIDO.\n");
            }
            else
                printf ("Error: la cadena recibida es incorrecta \n");
        }
    }
}
