# 💻 TdC - Trabajo Práctico N° 3: Simulador de Autómatas (Algoritmo de Thompson)

![C](https://img.shields.io/badge/Lenguaje-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Estado](https://img.shields.io/badge/Estado-Completado-brightgreen?style=for-the-badge)

## 📝 Descripción del Proyecto
Este repositorio contiene la resolución del **TP3** para la materia Teoría de la Computación. El objetivo de este proyecto es implementar un simulador de Autómatas Finitos No Deterministas (AFN) a partir de una Expresión Regular utilizando el **Algoritmo de Construcción de Thompson**. 

[cite_start]El programa procesa una expresión regular ingresada por el usuario, la convierte para su evaluación matemática y construye el autómata dinámicamente, permitiendo luego evaluar distintas cadenas de texto para comprobar si son aceptadas o rechazadas[cite: 49, 50, 54, 55].

---

## ⚙️ Funcionalidades Principales

* [cite_start]🔹 **Soporte de Operadores:** El simulador soporta operadores de clausura de Kleene (`*`), clausura positiva (`+`), unión o alternancia (`|`) y agrupaciones con paréntesis `()`[cite: 13].
* [cite_start]🔹 **Concatenación Implícita:** El algoritmo analiza la expresión regular e inyecta automáticamente un punto (`.`) para representar las concatenaciones implícitas entre caracteres[cite: 13, 14].
* [cite_start]🔹 **Conversión Posfija:** Transforma la expresión regular tradicional (infija) a notación posfija (Reverse Polish Notation) para facilitar la construcción del autómata respetando la precedencia de los operadores[cite: 15].
* [cite_start]🔹 **Transiciones Épsilon:** Las transiciones vacías o épsilon (ε) están representadas internamente por la letra `'E'`[cite: 2].
* [cite_start]🔹 **Reporte Detallado:** Al construir el autómata, el programa imprime por consola la cantidad total de estados creados, el estado inicial, el estado final y la tabla de transiciones completa[cite: 50, 51, 35].
* [cite_start]🔹 **Simulación Interactiva:** Una vez construido el AFN, el usuario puede ingresar cadenas de texto continuamente para verificar si son aceptadas o rechazadas por el lenguaje, hasta que se ingresa la palabra `salir` para terminar la ejecución[cite: 52, 53].

---

## 🏗️ Estructura Interna y Lógica

El código (`main.c`) está estructurado utilizando memoria dinámica e implementa las siguientes bases:

1. [cite_start]**Estructuras de Datos:** Se utilizan structs para definir cada `Estado` (con su ID y bandera de estado final) y cada `Transicion` (con su símbolo y puntero al estado destino)[cite: 1, 3].
2. [cite_start]**Pila de Fragmentos:** Para construir el autómata usando Thompson, se utiliza una pila de `FragmentoAFN` (que contiene un estado inicial y uno final) donde se van apilando y desapilando las sub-expresiones procesadas[cite: 7, 8].
3. **Construcción:** La función `construirAutomataThompson` evalúa la expresión posfija carácter por carácter. [cite_start]Si es un operando, crea un fragmento base[cite: 33]. [cite_start]Si es un operador (`.`, `|`, `*`, `+`), desapila los fragmentos necesarios, aplica las reglas de Thompson conectándolos con transiciones `'E'` y vuelve a apilar el resultado[cite: 24, 25, 26, 27, 28, 29, 30].

---

## 🖼️ Demostración y Funcionamiento

A continuación se muestra un ejemplo de la ejecución del programa por consola:

![Ejecución del programa](https://via.placeholder.com/800x400.png?text=Ac%C3%A1+arrastr%C3%A1+una+captura+de+tu+consola+corriendo+el+c%C3%B3digo)
*(Nota: Arrastrá y soltá en GitHub una captura de pantalla de tu terminal mostrando la tabla de transiciones y la evaluación de cadenas para reemplazar esta imagen)*

