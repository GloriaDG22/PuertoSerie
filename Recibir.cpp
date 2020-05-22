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
    colorRecibo = 5+7*16; ///Recibo: letra morado (5) y fondo gris claro (7)
    esFichero=false;
    finFichero=false;
    fRecibo=fRecibo->getInstance();
    pRecibo=pRecibo->getInstance();
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
    unsigned char tipoTrama=0;
    if (carR){
        switch (campoT){
        case 1: //sincronización (22)
            switch (carR){
            case 22: ///es una trama
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
                fRecibo->cerrarFlujo(); //cerramos el fichero log de F5
                pRecibo->iniciarVariables();
                SetConsoleTextAttribute(Pantalla, 14);
                pRecibo->iniciarProtMaestro(PuertoCOM, Pantalla);
                break;
            case 'E':
                fRecibo->cerrarFlujo(); //cerramos el fichero log de F5
                pRecibo->iniciarVariables();
                SetConsoleTextAttribute(Pantalla, 14);
                pRecibo->iniciarProtEsclavo(PuertoCOM, Pantalla);
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
                SetConsoleTextAttribute(Pantalla, colorRecibo);
                campoT=1;
                if(!pRecibo->getEsProt()){
                    fRecibo->escribirCadena ("Se ha recibido una trama de tipo ");
                    tRecibida.imprimirTipoTrama();
                }
                else{
                    if (tRecibida.getControl()==4)
                        pRecibo->setColor(CCIERRE);
                    SetConsoleTextAttribute(Pantalla, pRecibo->getColor());
                    pRecibo->imprimirTrama(tRecibida.getControl(), 0, tRecibida.getDir(), tRecibida.getNumTrama(), "R");
                }
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
            unsigned char bce;
            if((bce=tRecibida.calcularBce())==(unsigned char)carR){
                pRecibo->setTCorrecta(true);
                if (esFichero){
                    procesarFichero(Pantalla);
                }
                else{
                    if (finFichero){
                        string fin = "\nFichero recibido.\n";
                        string cadena = "El fichero recibido tiene un tamaño de " + (string)tRecibida.getDatos() + " bytes.\n";
                        if(pRecibo->getEsProt()){
                            pRecibo->escribirCadena(fin);
                            pRecibo->escribirCadena(cadena);
                            pRecibo->setFinTransferencia(true);
                            pRecibo->setColor(CTRANSFERENCIA3);
                            SetConsoleTextAttribute(Pantalla, pRecibo->getColor());
                        }
                        else{
                            fRecibo->escribirCadena(fin);
                            fRecibo->escribirCadena(cadena);
                        }
                        finFichero=false;
                    }
                    else{
                        SetConsoleTextAttribute(Pantalla, colorRecibo);
                        tRecibida.mostrarTrama();
                    }
                }
            }
            else{
                if (!pRecibo->getEsProt()){
                    if (esFichero)
                        fRecibo->escribirCadena ("Error en la recepcion de la trama del fichero.\n");
                    else{
                        SetConsoleTextAttribute(Pantalla, colorRecibo);
                        fRecibo->escribirCadena ("Error en la trama recibida.\n");
                    }
                }
            }
            tipoTrama=tRecibida.getControl();
            if (pRecibo->getEsProt()){
                SetConsoleTextAttribute(Pantalla, pRecibo->getColor());
                pRecibo->imprimirTrama(tRecibida.getControl(), tRecibida.getBCE(), tRecibida.getDir(), tRecibida.getNumTrama(), "R");
                pRecibo->escribirEntero((int)bce);
            }
            break;
        }
    }
    return tipoTrama;
}

void Recibir::procesarFichero(HANDLE Pantalla){
    switch (lineaFichero){
    SetConsoleTextAttribute(Pantalla, pRecibo->getColor());
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
            string cadena = "Recibiendo fichero por " + (string)autores + ".";
            if (!pRecibo->getEsProt())
                fRecibo->escribirCadena(cadena);
            else
                pRecibo->escribirCadena(cadena);
        }
        lineaFichero++;
        break;
    default:
        if(fSal.is_open())
            fSal.write(tRecibida.getDatos(), tRecibida.getLong());
        break;
    }
}

bool Recibir::getEsProt(){
    return pRecibo->getEsProt();
}

Recibir::~Recibir(){
    if(fRecibo!=0)
        delete fRecibo;
    if(pRecibo!=0)
        delete pRecibo;
    if(color!=NULL)
        delete color;
}
