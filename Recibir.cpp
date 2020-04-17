//============================================================================
// ------------ Gloria Méndez Sánchez y Gloria Díaz González------------------
//============================================================================

#include "Recibir.h"

Recibir* Recibir::obj=0;

Recibir::Recibir (){
    campoT=1;
    tRecibida= Trama();
    autores[0]='\0';
    color=new char;
    nomFichero[0]='\0';
    lineaFichero=1;
    colorFichero=3;
    colorRecibo = 5+7*16; ///Recibo: letra morado (5) y fondo gris claro (7)
    esFichero=false;
    finFichero=false;
    esProt=false;
    fRecibo=fRecibo->getInstance();
}

void Recibir::createInstance (){
    if (obj==0)
        obj = new Recibir();
}

Recibir* Recibir::getInstance (){
    createInstance();
    return obj;
}

unsigned char Recibir::recibir(char carR, HANDLE &PuertoCOM, HANDLE &Pantalla){
    if (carR){
        switch (campoT){
        unsigned char tipoTrama;
        case 1: ///sincronización (22)
            switch (carR){
            case 22: ///es una trama
                tipoTrama=0;
                tRecibida.setSincr(carR);
                campoT++;
                break;
            case '{':
                esFichero=true;
                break;
            case '}':
                fSal.close();
                esFichero=false;
                finFichero=true;
                lineaFichero=1;
                break;
            case 'M':
                ProtMaestro prot = Protocolo();
                prot.iniciarProtocolo();
                esProt=true;
                break;
            case 'E':
                ProtEsclavo prot = Protocolo();
                prot.iniciarProtocolo();
                esProt=true;
                break;
            }
            break;
        case 2: //dirección ('T')
            tRecibida.setDir(carR);
            campoT ++;
            break;
        case 3: //control ENQ-5  EOT-4  ACK-6  NACK-21 / DATOS-2
            tRecibida.setControl(carR);
            campoT++;
            break;
        case 4: //numero de trama (0)
            tRecibida.setNumTrama(carR);
            if (tRecibida.getControl()!=2){
                tipoTrama=tRecibida.getControl();
                campoT=1;
                SetConsoleTextAttribute(Pantalla, colorRecibo);
                fRecibo->escribirCadena ("Se ha recibido una trama de tipo ");
                tRecibida.imprimirTipoTrama();
            }
            else
                campoT++;
            break;
        case 5: //longitud
            tRecibida.setLong((unsigned char)carR);
            campoT++;
        case 6: //datos
            char cadenaRecibida [254];
            RecibirCadena(PuertoCOM, cadenaRecibida, tRecibida.getLong());
            tRecibida.setDatos(cadenaRecibida);
            campoT++;
            break;
        case 7: //BCE
            campoT=1;
            tRecibida.setBCE((unsigned char)carR);
            SetConsoleTextAttribute(Pantalla, colorFichero);
            prot.setFinRecibo (true);
            if(tRecibida.calcularBce()==(unsigned char)carR){
                tipoTrama = tRecibida.getControl();
                if (esFichero){
                    procesarFichero(Pantalla);
                }
                else{
                    if (finFichero){
                        fRecibo->escribirCadena("Fichero recibido.\n");
                        string cadena = "El fichero recibido tiene un tamaño de " + (string)tRecibida.getDatos() + " bytes.\n";
                        fRecibo->escribirCadena(cadena);
                        finFichero=false;
                    }
                    else{
                        SetConsoleTextAttribute(Pantalla, colorRecibo);
                        tRecibida.mostrarTrama();
                    }
                }
            }
            else{
                if (esFichero)
                    fRecibo->escribirCadena ("Error en la recepción de la trama del fichero.\n");
                else{
                    SetConsoleTextAttribute(Pantalla, colorRecibo);
                    fRecibo->escribirCadena ("Error en la trama recibida.\n");
                }
            }
            break;
        }
    }
    return tipoTrama;
}

void Recibir::procesarFichero(HANDLE Pantalla){
    switch (lineaFichero){
    case 1:
        strcpy(autores, tRecibida.getDatos());
        lineaFichero++;
        break;
    case 2:
        strcpy(color, tRecibida.getDatos());
        colorFichero=atoi(color);
        lineaFichero++;
        break;
    case 3:
        strcpy(nomFichero, tRecibida.getDatos());
        fSal.open(nomFichero);
        if(!fSal.is_open())
            fRecibo->escribirCadena("Error al abrir el fichero de escritura.\n");
        else{
            string cadena = "Recibiendo fichero por " + (string)autores + ".\n";
            fRecibo->escribirCadena(cadena);
        }
        lineaFichero++;
        break;
    default:
        if(fSal.is_open()){
            fSal.write(tRecibida.getDatos(), tRecibida.getLong());
        }
        break;
    }
}

void Recibir::setEsPRot (bool _esProt){
    esProt = _esProt;
}

