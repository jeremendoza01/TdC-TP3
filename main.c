#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Estado Estado;
typedef struct Transicion Transicion;

struct Transicion
{
    char simbolo; // 'E' representará a Épsilon (ε)
    Estado *destino;
    Transicion *siguiente;
};

struct Estado
{
    int id;
    int esFinal;
    Transicion *transiciones;
};

int cantidadEstados = 0;
Estado *listaEstados[1000];

Estado *crearEstado(int esFinal)
{
    Estado *nuevoEstado = (Estado *)malloc(sizeof(Estado));
    nuevoEstado->id = cantidadEstados;
    nuevoEstado->esFinal = esFinal;
    nuevoEstado->transiciones = NULL;

    listaEstados[cantidadEstados] = nuevoEstado;
    cantidadEstados++;

    return nuevoEstado;
}

void agregarTransicion(Estado *origen, char simbolo, Estado *destino)
{
    Transicion *nuevaTransicion = (Transicion *)malloc(sizeof(Transicion));
    nuevaTransicion->simbolo = simbolo;
    nuevaTransicion->destino = destino;
    nuevaTransicion->siguiente = origen->transiciones;
    origen->transiciones = nuevaTransicion;
}

// 2. CONSTRUCCIÓN DE THOMPSON
typedef struct
{
    Estado *estadoInicial;
    Estado *estadoFinal;
} FragmentoAFN;

FragmentoAFN pilaFragmentos[100];
int topePila = -1;

void apilarFragmento(FragmentoAFN fragmento) { pilaFragmentos[++topePila] = fragmento; }
FragmentoAFN desapilarFragmento() { return pilaFragmentos[topePila--]; }

FragmentoAFN crearFragmentoBase(char simbolo)
{
    Estado *inicio = crearEstado(0);
    Estado *fin = crearEstado(1);
    agregarTransicion(inicio, simbolo, fin);
    FragmentoAFN fragmento = {inicio, fin};
    return fragmento;
}

// 3. PARSER: INFIJA A POSFIJA
void inyectarConcatenacion(const char *expresionRegular, char *expresionSalida)
{
    int indiceSalida = 0;
    for (int i = 0; expresionRegular[i]; i++)
    {
        expresionSalida[indiceSalida++] = expresionRegular[i];
        if (expresionRegular[i + 1])
        {
            char charActual = expresionRegular[i];
            char charSiguiente = expresionRegular[i + 1];
            if (charActual != '(' && charActual != '|' &&
                charSiguiente != ')' && charSiguiente != '|' && charSiguiente != '*' && charSiguiente != '+')
            {
                expresionSalida[indiceSalida++] = '.'; // El punto representa la concatenación implícita
            }
        }
    }
    expresionSalida[indiceSalida] = '\0';
}

void infijaAPosfija(const char *expresionInfija, char *expresionPosfija)
{
    char pilaOperadores[100];
    int topeOperadores = -1;
    int indicePosfija = 0;

    for (int i = 0; expresionInfija[i]; i++)
    {
        char caracterActual = expresionInfija[i];
        if (isalnum(caracterActual))
        {
            expresionPosfija[indicePosfija++] = caracterActual;
        }
        else if (caracterActual == '(')
        {
            pilaOperadores[++topeOperadores] = caracterActual;
        }
        else if (caracterActual == ')')
        {
            while (topeOperadores >= 0 && pilaOperadores[topeOperadores] != '(')
                expresionPosfija[indicePosfija++] = pilaOperadores[topeOperadores--];
            topeOperadores--;
        }
        else if (caracterActual == '*' || caracterActual == '+')
        {
            expresionPosfija[indicePosfija++] = caracterActual;
        }
        else if (caracterActual == '.')
        {
            while (topeOperadores >= 0 && (pilaOperadores[topeOperadores] == '.' || pilaOperadores[topeOperadores] == '*'))
                expresionPosfija[indicePosfija++] = pilaOperadores[topeOperadores--];
            pilaOperadores[++topeOperadores] = caracterActual;
        }
        else if (caracterActual == '|')
        {
            while (topeOperadores >= 0 && pilaOperadores[topeOperadores] != '(')
                expresionPosfija[indicePosfija++] = pilaOperadores[topeOperadores--];
            pilaOperadores[++topeOperadores] = caracterActual;
        }
    }
    while (topeOperadores >= 0)
        expresionPosfija[indicePosfija++] = pilaOperadores[topeOperadores--];
    expresionPosfija[indicePosfija] = '\0';
}

Estado *construirAutomataThompson(const char *expresionPosfija, Estado **estadoFinalOut)
{
    cantidadEstados = 0;
    topePila = -1;

    for (int i = 0; expresionPosfija[i]; i++)
    {
        char caracterActual = expresionPosfija[i];
        if (caracterActual == '.')
        {
            FragmentoAFN fragDer = desapilarFragmento();
            FragmentoAFN fragIzq = desapilarFragmento();
            fragIzq.estadoFinal->esFinal = 0;
            agregarTransicion(fragIzq.estadoFinal, 'E', fragDer.estadoInicial);
            FragmentoAFN fragNuevo = {fragIzq.estadoInicial, fragDer.estadoFinal};
            apilarFragmento(fragNuevo);
        }
        else if (caracterActual == '|')
        {
            FragmentoAFN fragDer = desapilarFragmento();
            FragmentoAFN fragIzq = desapilarFragmento();
            Estado *nuevoInicio = crearEstado(0);
            Estado *nuevoFin = crearEstado(1);
            fragIzq.estadoFinal->esFinal = 0;
            fragDer.estadoFinal->esFinal = 0;
            agregarTransicion(nuevoInicio, 'E', fragIzq.estadoInicial);
            agregarTransicion(nuevoInicio, 'E', fragDer.estadoInicial);
            agregarTransicion(fragIzq.estadoFinal, 'E', nuevoFin);
            agregarTransicion(fragDer.estadoFinal, 'E', nuevoFin);
            FragmentoAFN fragNuevo = {nuevoInicio, nuevoFin};
            apilarFragmento(fragNuevo);
        }
        else if (caracterActual == '*')
        {
            FragmentoAFN fragBase = desapilarFragmento();
            Estado *nuevoInicio = crearEstado(0);
            Estado *nuevoFin = crearEstado(1);
            fragBase.estadoFinal->esFinal = 0;
            agregarTransicion(nuevoInicio, 'E', fragBase.estadoInicial);
            agregarTransicion(nuevoInicio, 'E', nuevoFin);
            agregarTransicion(fragBase.estadoFinal, 'E', fragBase.estadoInicial);
            agregarTransicion(fragBase.estadoFinal, 'E', nuevoFin);
            FragmentoAFN fragNuevo = {nuevoInicio, nuevoFin};
            apilarFragmento(fragNuevo);
        }
        else if (caracterActual == '+')
        {
            FragmentoAFN fragBase = desapilarFragmento();
            Estado *nuevoInicio = crearEstado(0);
            Estado *nuevoFin = crearEstado(1);
            fragBase.estadoFinal->esFinal = 0;
            agregarTransicion(nuevoInicio, 'E', fragBase.estadoInicial);
            agregarTransicion(fragBase.estadoFinal, 'E', fragBase.estadoInicial);
            agregarTransicion(fragBase.estadoFinal, 'E', nuevoFin);
            FragmentoAFN fragNuevo = {nuevoInicio, nuevoFin};
            apilarFragmento(fragNuevo);
        }
        else
        {
            apilarFragmento(crearFragmentoBase(caracterActual));
        }
    }
    FragmentoAFN automataFinal = desapilarFragmento();
    *estadoFinalOut = automataFinal.estadoFinal;
    return automataFinal.estadoInicial;
}

void imprimirTablaTransiciones()
{
    printf("Tabla de transiciones:\n");
    for (int i = 0; i < cantidadEstados; i++)
    {
        Estado *estadoActual = listaEstados[i];
        if (!estadoActual->transiciones)
            continue;

        char simbolosImpresos[256] = {0};

        for (Transicion *t1 = estadoActual->transiciones; t1; t1 = t1->siguiente)
        {
            unsigned char sim = (unsigned char)t1->simbolo;
            if (simbolosImpresos[sim])
                continue;
            simbolosImpresos[sim] = 1;

            // Imprime el formato origen --simbolo-->
            printf("q%d --%c--> ", estadoActual->id, t1->simbolo);

            int primerDestino = 1;
            // Busca todos los destinos que corresponden a ese símbolo y los separa por comas
            for (Transicion *t2 = estadoActual->transiciones; t2; t2 = t2->siguiente)
            {
                if (t2->simbolo == t1->simbolo)
                {
                    if (!primerDestino)
                        printf(", ");
                    printf("q%d", t2->destino->id);
                    primerDestino = 0;
                }
            }
            printf("\n");
        }
    }
}

int simularAFN(Estado *estadoActual, const char *cadenaEvaluar, int posicionCadena)
{
    if (posicionCadena == strlen(cadenaEvaluar))
    {
        if (estadoActual->esFinal)
            return 1;
        for (Transicion *t = estadoActual->transiciones; t; t = t->siguiente)
        {
            if (t->simbolo == 'E' && simularAFN(t->destino, cadenaEvaluar, posicionCadena))
                return 1;
        }
        return 0;
    }
    char caracterEvaluar = cadenaEvaluar[posicionCadena];
    for (Transicion *t = estadoActual->transiciones; t; t = t->siguiente)
    {
        if (t->simbolo == caracterEvaluar && simularAFN(t->destino, cadenaEvaluar, posicionCadena + 1))
            return 1;
        if (t->simbolo == 'E' && simularAFN(t->destino, cadenaEvaluar, posicionCadena))
            return 1;
    }
    return 0;
}

int main()
{
    char expresionRegular[100], expresionConPuntos[200], expresionPosfija[200];

    printf("=== SIMULADOR DE AUTOMATA - ALGORITMO DE THOMPSON ===\n");
    printf("Ingrese expresion regular: ");
    scanf("%99s", expresionRegular);

    inyectarConcatenacion(expresionRegular, expresionConPuntos);
    infijaAPosfija(expresionConPuntos, expresionPosfija);

    printf("Construyendo automata....\n");
    Estado *estadoFinalAutomata = NULL;
    Estado *estadoInicialAutomata = construirAutomataThompson(expresionPosfija, &estadoFinalAutomata);

    printf("Estados creados: %d\n", cantidadEstados);
    printf("Estado inicial: q%d\n", estadoInicialAutomata->id);
    printf("Estado final: q%d\n", estadoFinalAutomata->id);

    imprimirTablaTransiciones();

    getchar(); // Limpiar el buffer del scanner

    char cadenaEvaluar[256];
    while (1)
    {
        printf("\nIngrese cadena (o 'salir'): ");
        fgets(cadenaEvaluar, sizeof(cadenaEvaluar), stdin);
        cadenaEvaluar[strcspn(cadenaEvaluar, "\n")] = 0; // Quita el salto de línea

        if (strcmp(cadenaEvaluar, "salir") == 0)
            break;

        if (simularAFN(estadoInicialAutomata, cadenaEvaluar, 0))
        {
            printf(" -> ACEPTADA\n");
        }
        else
        {
            printf(" -> RECHAZADA\n");
        }
    }
    return 0;
}