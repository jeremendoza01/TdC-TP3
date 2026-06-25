# 💻 TdC - Trabajo Práctico N° 3: Simulador de Autómatas (Algoritmo de Thompson)

![C](https://img.shields.io/badge/Lenguaje-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Estado](https://img.shields.io/badge/Estado-Completado-brightgreen?style=for-the-badge)

## 📝 Descripción del Proyecto
Este repositorio contiene la resolución del **TP3** para la materia Teoría de la Computación. El objetivo de este proyecto es implementar un simulador de Autómatas Finitos No Deterministas (AFN) a partir de una Expresión Regular utilizando el **Algoritmo de Construcción de Thompson**. 

El programa procesa una expresión regular ingresada por el usuario, la convierte para su evaluación matemática y construye el autómata dinámicamente, permitiendo luego evaluar distintas cadenas de texto para comprobar si son aceptadas o rechazadas.

---

## ⚙️ Funcionalidades Principales

* 🔹 **Soporte de Operadores:** El simulador soporta operadores de clausura de Kleene (`*`), clausura positiva (`+`), unión o alternancia (`|`) y agrupaciones con paréntesis `()`.
* 🔹 **Concatenación Implícita:** El algoritmo analiza la expresión regular e inyecta automáticamente un punto (`.`) para representar las concatenaciones implícitas entre caracteres.
* 🔹 **Conversión Posfija:** Transforma la expresión regular tradicional (infija) a notación posfija (Reverse Polish Notation) para facilitar la construcción del autómata respetando la precedencia de los operadores.
* 🔹 **Transiciones Épsilon:** Las transiciones vacías o épsilon (ε) están representadas internamente por la letra `'E'`.
* 🔹 **Reporte Detallado:** Al construir el autómata, el programa imprime por consola la cantidad total de estados creados, el estado inicial, el estado final y la tabla de transiciones completa.
* 🔹 **Simulación Interactiva:** Una vez construido el AFN, el usuario puede ingresar cadenas de texto continuamente para verificar si son aceptadas o rechazadas por el lenguaje, hasta que se ingresa la palabra `salir` para terminar la ejecución.

---

## 🏗️ Estructura Interna y Lógica

El código (`main.c`) está estructurado utilizando memoria dinámica e implementa las siguientes bases:

1. **Estructuras de Datos:** Se utilizan structs para definir cada `Estado` (con su ID y bandera de estado final) y cada `Transicion` (con su símbolo y puntero al estado destino).
2. **Pila de Fragmentos:** Para construir el autómata usando Thompson, se utiliza una pila de `FragmentoAFN` (que contiene un estado inicial y uno final) donde se van apilando y desapilando las sub-expresiones procesadas.
3. **Construcción:** La función `construirAutomataThompson` evalúa la expresión posfija carácter por carácter. Si es un operando, crea un fragmento base. Si es un operador (`.`, `|`, `*`, `+`), desapila los fragmentos necesarios, aplica las reglas de Thompson conectándolos con transiciones `'E'` y vuelve a apilar el resultado.
