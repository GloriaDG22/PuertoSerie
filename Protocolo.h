#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#define ProtMaestro "Prolog-m.txt"
#define ProtEsclavo "Prolog-e.txt"

class Protocolo
{

    private:
        char estacion; //E: esclavo, M: maestro
        FILE *ficheroProt;

    public:
        iniciarProtocolo();
        Protocolo();
        virtual ~Protocolo();
};

#endif // PROTOCOLO_H
