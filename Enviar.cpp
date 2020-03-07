#include "Enviar.h"

Enviar::Enviar(){
    caracter = ' ';
    cont = 0;
}

Enviar::~Enviar(){
}

int Enviar::getCont(){
    return cont;
}

void Enviar::setCont(int valor){
    cont=valor;
}

void Enviar::comprobarTeclaFuncion(char carE, HANDLE PuertoCOM){
    switch (carE){
        case 59:
            printf("\n");
            addChar('\n');
            addChar('\0');
            crearTramaDatos(PuertoCOM);
                //dividirCadena(cont, numTramas);
                //enviarTrama(PuertoCOM);
            // a continuacion, metemos en el vector Datos los caracteres
            // que hemos escrito y añadimos un nuevo \0 para marcar el final de la cadena. si el resultado del bce es 0 o 255,
            // como esos valores no pueden enviarse ppor el puerto, le ponemos manualmente un 1.
            // para enviar la trama tenemos que ir enviando los campos por separado: enviarCaracter para todos los campos unsigned char excepto
            // para el char datos que usaremos el enviarCadena
            // para fragmentar el mensaje (si es de mas de 254 caracteres) tendremos que calcular el numero de tramas que vamos a enviar:
            // numero de caracteres que ha escrito el usuario / 254
            // y con el valor resultamente hacemos un bucle que envie esas x tramas
            // for (i=0; i< numTramas; i++){
            //      construirTrama() : construimos la trama metiendole los valores correspondientes en cada trama y calculamos el bce con el campo de datos correspondiente
            //      enviarTrama() : enviamos la trama que acabamos de construir
            // }
            //si enviamos un mensaje vacio tenemos que enviar una trama de datos de longitud 1 que tendria solo un \ng
            //EnviarCadena(PuertoCOM, cadena, cont);
            cont=0;
            break;
        case 60:
            crearTramaControl(PuertoCOM);
    }
}

void Enviar::enviarCaracter(HANDLE PuertoCOM, char carE){
    printf("%c", carE);   //Envío
    EnviarCaracter(PuertoCOM,caracter);
}

void Enviar::enviarCadena (char carE){
    switch (carE){
    case 8:
        if(cadena[cont-1]!='\n'){
            //printf("\b \b");
            printf("%c", carE);
            printf(" ");
            printf("%c", carE);
            cont--;
        }
        break;
    case 13:
        if(cont < MAX){
            printf("\n");
            cadena[cont] = '\n';
            cont++;
        }
        break;
    default:
        if(cont < MAX){
            printf("%c", carE);
            cadena[cont]=carE;
            cont++;
        }
        break;
    }
}

void Enviar::addChar(char carE){
    cadena[cont]=carE;
    cont++;
}

void Enviar::crearTramaControl(HANDLE PuertoCOM){
    printf ("\nElija el tipo de trama que quiere enviar \n 1: Trama ENQ \n 2: Trama EOT \n 3: Trama ACK \n 4: Trama NACK \n");
    int opcionT;
    unsigned char control;
    bool valorCorrecto=false;
    bool envio=true;
    while (!valorCorrecto){
        opcionT = getch();
        switch (opcionT){
            case '1': //Tipo ENQ
                control = 05;
                valorCorrecto = true;
                break;
            case '2': //Tipo EOT
                control = 04;
                valorCorrecto = true;
                break;
            case '3': //Tipo ACK
                control = 06;
                valorCorrecto = true;
                break;
            case '4': //Tipo NACK
                control = 21;
                valorCorrecto = true;
                break;
            case 27:
                printf ("Se ha cancelado el envio de la trama \n");
                valorCorrecto = true;
                envio = false;
                break;
            default:
                 printf("Ha introducido un valor erroneo, intentelo de nuevo \n");
                break;
        }
    }
    if (envio==true){
        Trama t=Trama();
        t.setControl(control);
        enviarTrama(t, PuertoCOM);
    }
}

void Enviar::enviarTrama(Trama t, HANDLE PuertoCOM){
    EnviarCaracter(PuertoCOM, t.getSincr());
    EnviarCaracter(PuertoCOM, t.getDir());
    EnviarCaracter(PuertoCOM, t.getControl());
    EnviarCaracter(PuertoCOM, t.getNumTrama());
    if (t.getControl()==2){
        EnviarCaracter(PuertoCOM, t.getLong());
        EnviarCadena(PuertoCOM, t.getDatos(), t.getLong());
        EnviarCaracter(PuertoCOM, t.getBCE());
    }
    else{
        printf ("Se ha enviado una trama de tipo ");
        t.imprimirTipoTrama();
    }
}

void Enviar::crearTramaDatos(HANDLE PuertoCOM){
    Trama t=Trama(), aux;
    int offset;
    char cadenaEnvio [255], carR;
    int numTramas, campoT=1;
    dividirCadena(cont, numTramas);
    for (int i=0; i<numTramas; i++){
        t.setControl(2);
        offset = 254 * (i);
        copiarCadena (cadena, cadenaEnvio, offset, 254);
        t.setLong(strlen(cadenaEnvio));
        t.setDatos(cadenaEnvio);
        t.setBCE(t.calcularBce());
        enviarTrama(t, PuertoCOM);
        carR = RecibirCaracter(PuertoCOM);
        recibo.recibir(carR, campoT, aux, PuertoCOM);
    }
}

void Enviar::dividirCadena(int cont, int &numTramas){
    if (cont%254==0)
        numTramas=cont/254;
    else
        numTramas=(cont/254)+1;
}

void Enviar::copiarCadena (const char* cadenaFuente, char* cadenaDestino, int offset, int longit){
    int i = 0;
    char actual;
    do{
        actual = cadenaFuente [i+offset];
        cadenaDestino[i] = actual;
        i++;
    } while ((actual!='\0')&&(i<longit));
    cadenaDestino[i]='\0';
}
