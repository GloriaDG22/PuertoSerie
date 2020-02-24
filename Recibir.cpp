#include "Recibir.h"

void Recibir::recibir(char carR, int &campoT, Trama &aux){
    if (carR){
        switch (campoT){
        case 1:
            if (carR==22){ //es una trama
                aux.setSincr(carR);
                campoT++;
            }
            else{ //no es una trama
                printf("%c", carR);
            }
            break;
        case 2:
            aux.setDir(carR);
            campoT ++;
            break;
        case 3:
            aux.setControl(carR);
            campoT++;
            break;
        case 4:
            aux.setNumTrama(carR);
            campoT=1;
            aux.imprimirTipoTrama();
            break;
        }
    }
}
