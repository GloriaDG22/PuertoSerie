#include "Recibir.h"

void Recibir::recibir(char carR, int &campoT, Trama &aux, HANDLE PuertoCOM){
    if (carR){
        switch (campoT){
        case 1:
            if (carR==22){ //es una trama
                aux=Trama();
                campoT++;
            }
            printf ("Campo 1 \n");
            break;
        case 2:
            campoT ++;
            printf ("Campo 2 \n");
            break;
        case 3:
            aux.setControl(carR);
            campoT++;
            printf ("Campo 3 \n");
            break;
        case 4:
            if (aux.getControl()!=2){
                campoT=1;
                printf ("Se ha recibido una trama de tipo ");
                aux.imprimirTipoTrama();
                printf ("if del campo 4 (control) \n");
            }
            else{
                campoT++;
                printf ("else del campo 4 (datos) \n");
            }
            break;
        case 5:
            aux.setLong((unsigned char)carR);
            campoT++;
            printf ("Campo 5 \n");
        case 6:
            printf("Inicio campo 6");
            char cadenaRecibida [254];
            RecibirCadena(PuertoCOM, cadenaRecibida, aux.getLong());
            aux.setDatos(cadenaRecibida);
            campoT++;
            printf ("Campo 6 \n");
            break;
        case 7:
            campoT=1;
            aux.setBCE((unsigned char)carR);
            printf ("Campo 7 \n");
            if(aux.calcularBce()==(unsigned char)carR){
                aux.mostrarTrama();
            }
            else
                printf ("Error: la cadena recibida es incorrecta \n");
        }
    }
}
