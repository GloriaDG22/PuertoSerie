//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Recibir.h"

Recibir* Recibir::obj=0;

Recibir::Recibir (){
    autores[0]='\0';
    color[0]='\0';
    nomFichero[0]='\0';
    lineaFichero=1;
    int colorTexto=0;
    esFichero=false;
    finFichero=false;
}

void Recibir::createInstance (){
    if (obj==0)
        obj = new Recibir();
}

Recibir* Recibir::getInstance (){
    createInstance();
    return obj;
}

void Recibir::recibir(char carR, int &campoT, Trama &aux, HANDLE PuertoCOM, HANDLE Pantalla){
    if (carR){
        switch (campoT){
        case 1: //sincronización (22)
            if (carR==22){ //es una trama
                aux=Trama();
                aux.setSincr(carR);
                campoT++;
            }
            else{
                if (carR == '{'){
                    esFichero=true;
                }
                else{
                    if(carR == '}'){
                        fSal.close();
                        esFichero=false;
                        finFichero=true;
                        lineaFichero=1;
                    }
                }
            }
            break;
        case 2: //dirección ('T')
            aux.setDir(carR);
            campoT ++;
            break;
        case 3: //control ENQ-5  EOT-4  ACK-6  NACK-21 / DATOS-2
            aux.setControl(carR);
            campoT++;
            break;
        case 4: //numero de trama (0)
            aux.setNumTrama(carR);
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
            SetConsoleTextAttribute(Pantalla, colorTexto);
            aux.setBCE((unsigned char)carR);
            if(aux.calcularBce()==(unsigned char)carR){
                if (esFichero)
                    procesarFichero(Pantalla, aux, fSal);
                else{
                    if (finFichero){
                        printf ("Fichero recibido.\n");
                        printf ("El fichero recibido tiene un tamaño de %s bytes.\n", aux.getDatos());
                        finFichero=false;
                    }
                    else
                        aux.mostrarTrama();
                }
            }
            else{
                if (esFichero)
                    printf ("Error en la recepción de la trama del fichero.\n");
                else
                    printf ("Error en la trama recibida.\n");
            }
            break;
        }
    }
}

void Recibir::procesarFichero(HANDLE Pantalla, Trama t, ofstream fSal){
    switch (lineaFichero){
    case 1:
        strcpy(autores, t.getDatos());
        lineaFichero++;
        break;
    case 2:
        strcpy(color, t.getDatos());
        int texto=color[0]-'0', fondo=color[1]-'0';
        colorTexto=texto+fondo*16;
        lineaFichero++;
        break;
    case 3:
        strcpy(nomFichero, t.getDatos());
        fSal.open(nomFichero);
        if(!fSal.is_open())
            printf("Error al abrir el fichero de escritura.\n");
        else{
            printf("Recibiendo fichero por %s.\n", autores);
            SetConsoleTextAttribute(Pantalla, colorTexto);
        }
        lineaFichero++;
        break;
    default:
        if(fSal.is_open()){
            fSal.write(t.getDatos(), t.getLong());
        }
        break;
    }
}


