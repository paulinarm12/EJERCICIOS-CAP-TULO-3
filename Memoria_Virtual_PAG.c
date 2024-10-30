/**
 * Nombre del equipo: S.O. AGREVAL
 * Fecha: 29/10/2024
 * Versión: 1.2.1
 * Descripción:
 * Este programa en C simula la gestión de memoria de un sistema operativo, en el que se analiza 
 * el tiempo de acceso promedio a memoria utilizando un TLB (Translation Lookaside Buffer) y un 
 * sistema de memoria virtual con tres niveles de tablas de páginas. El programa descompone 
 * direcciones virtuales de 36 bits en sus componentes y calcula el tiempo de acceso a memoria 
 * en base a una tasa de aciertos en el TLB.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_SIZE (1 << 12) // Tamaño de página de 4KB
#define ADDRESS_SIZE 36     // Dirección virtual de 36 bits
#define TLB_HIT_TIME 8      // Tiempo de acceso al TLB en nanosegundos (ns)
#define MEMORY_ACCESS_TIME 70 // Tiempo de acceso a la memoria principal en ns
#define TLB_HIT_RATE 0.9    // Tasa de aciertos en el TLB (90%)

// Estructura que representa los componentes de una dirección virtual descompuesta
typedef struct {
    unsigned int offset;      // Offset dentro de la página (últimos 12 bits)
    unsigned int lvl3_index;  // Índice de la tabla de páginas de nivel 3 (siguientes 8 bits)
    unsigned int lvl2_index;  // Índice de la tabla de páginas de nivel 2 (siguientes 8 bits)
    unsigned int lvl1_index;  // Índice de la tabla de páginas de nivel 1 (4 bits más significativos)
} VirtualAddress;

/**
 * Función: decompose_address
 * Descripción: Descompone una dirección virtual de 36 bits en los componentes
 *              necesarios para un sistema de memoria virtual con tres niveles
 *              de tablas de páginas. Cada nivel se accede mediante una parte
 *              de la dirección, y el offset indica la posición dentro de la
 *              página.
 * Parámetros:
 *   - virtual_address: dirección virtual en formato entero sin signo (32 bits).
 * Retorno:
 *   - Estructura VirtualAddress que contiene el offset y los índices de cada nivel.
 */
VirtualAddress decompose_address(unsigned int virtual_address) {
    VirtualAddress addr;

    // Los últimos 12 bits de la dirección representan el offset dentro de la página.
    addr.offset = virtual_address & ((1 << 12) - 1);

    // Los 8 bits siguientes representan el índice de la tabla de nivel 3.
    addr.lvl3_index = (virtual_address >> 12) & 0xFF;

    // Otros 8 bits representan el índice de la tabla de nivel 2.
    addr.lvl2_index = (virtual_address >> 20) & 0xFF;

    // Los 4 bits más significativos representan el índice de la tabla de nivel 1.
    addr.lvl1_index = (virtual_address >> 28) & 0xF;

    return addr;
}

/**
 * Función: calculate_memory_access_time
 * Descripción: Calcula el tiempo promedio de acceso a memoria considerando el
 *              tiempo de acceso al TLB y a la memoria principal. El cálculo se
 *              realiza solo para los casos en que no hay fallo de página.
 * Proceso:
 *   - Si hay un acierto en el TLB, el tiempo de acceso es el tiempo del TLB.
 *   - Si hay un fallo en el TLB, se debe acceder a las tres tablas de páginas
 *     y finalmente a la memoria principal.
 * Retorno:
 *   - Tiempo promedio de acceso a memoria en nanosegundos.
 */
double calculate_memory_access_time() {
    // Tiempo promedio de acceso al TLB considerando la tasa de aciertos
    double tlb_access_time = TLB_HIT_RATE * TLB_HIT_TIME;

    // Tiempo de acceso a las tres tablas de páginas en caso de fallo en el TLB
    double page_table_access_time = (1 - TLB_HIT_RATE) * (3 * MEMORY_ACCESS_TIME);

    // Tiempo de acceso a la memoria principal (lectura/escritura)
    double memory_access_time = MEMORY_ACCESS_TIME;

    // Tiempo promedio total: TLB + tablas de páginas (si falla el TLB) + memoria
    return tlb_access_time + page_table_access_time + memory_access_time;
}

/*
 * Función principal:
 *    - Solicita al usuario ingresar una dirección virtual en hexadecimal.
 *    - Descompone la dirección en sus índices de tabla y el offset correspondiente.
 *    - Calcula y muestra el tiempo promedio de acceso a memoria.
 */

int main() {
    unsigned int virtual_address;

    // Solicita al usuario ingresar una dirección virtual en formato hexadecimal (hasta 36 bits)
    printf("Ingrese una dirección virtual (en hexadecimal, hasta 36 bits): ");
    scanf("%x", &virtual_address);

    // Llamada a la función decompose_address para descomponer la dirección virtual en sus componentes
    VirtualAddress addr = decompose_address(virtual_address);

    // Muestra los componentes descompuestos de la dirección virtual
    printf("Descomposición de la dirección virtual:\n");
    printf(" - Índice de tabla de nivel 1: %u\n", addr.lvl1_index);
    printf(" - Índice de tabla de nivel 2: %u\n", addr.lvl2_index);
    printf(" - Índice de tabla de nivel 3: %u\n", addr.lvl3_index);
    printf(" - Offset dentro de la página: %u\n", addr.offset);

    // Calcula el tiempo promedio de acceso a memoria y muestra el resultado
    double access_time = calculate_memory_access_time();
    printf("Tiempo promedio de acceso a memoria (sin fallo de página): %.2f ns\n", access_time);

    return 0;
}
