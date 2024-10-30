/* 
* Nombre del equipo: S.O. AGREVAL
 * Fecha: 29/10/2024
 * Versión: 1.2.1
 * 
 * Descripción:
 * Este programa en C simula la gestión de memoria de un sistema operativo, permitiendo la 
 * conversión de una dirección virtual de 32 bits a una dirección física en memoria. La simulación 
 * utiliza una tabla de páginas que incluye bits de presencia y modificación, así como marcos de 
 * página. A partir de la dirección virtual proporcionada por el usuario, el programa calcula el 
 * número de página y el offset dentro de la página, y luego intenta acceder a la tabla de páginas 
 * para determinar si la página está en memoria física o en swap.
 */

#include <stdio.h>
#include <stdint.h>

#define PAGE_SIZE (1 << 12)  // Tamaño de página: 4KB (2^12 bytes)
#define VIRTUAL_ADDRESS_BITS 32 // Tamaño de dirección virtual: 32 bits
#define PHYSICAL_ADDRESS_BITS 21 // Tamaño de memoria física: 2^21 bytes

// Estructura que representa una entrada de la tabla de páginas
typedef struct {
    int presence_bit;     // Bit de presencia: indica si la página está en memoria física (1) o en swap (0)
    int modified_bit;     // Bit de modificado: indica si la página ha sido modificada
    int page_frame;       // Marco de página o bloque de swap
} PageTableEntry;

// Tabla de páginas de ejemplo, tal como se proporciona en el enunciado
PageTableEntry page_table[] = {
    {1, 1, 0},
    {0, 0, 8},
    {1, 0, 9},
    {1, 1, 14},
    {1, 0, 3},
    {1, 0, 7},
    {0, 1, 25},
    {0, 1, 16}
};

/**
 * Función: get_physical_address
 * Descripción: Calcula la dirección física correspondiente a una dirección virtual dada.
 * Parámetros:
 *   - virtual_address: dirección virtual de 32 bits.
 * Retorno:
 *   - Dirección física correspondiente si la página está en memoria física, o -1 si la página está en swap.
 */
int get_physical_address(uint32_t virtual_address) {
    // Obtener el número de página y el offset a partir de la dirección virtual
    int page_number = (virtual_address >> 12) & 0xFF; // Los 8 bits correspondientes al número de página
    int offset = virtual_address & (PAGE_SIZE - 1);    // Los últimos 12 bits para el offset

    // Verificar si el número de página está dentro de los límites de la tabla
    if (page_number >= sizeof(page_table) / sizeof(PageTableEntry)) {
        printf("Número de página fuera de los límites de la tabla.\n");
        return -1;
    }

    // Verificar el bit de presencia en la tabla de páginas
    PageTableEntry entry = page_table[page_number];
    if (entry.presence_bit == 0) {
        printf("La página está en swap, no en memoria física.\n");
        return -1; // Retorna -1 si la página está en swap
    }

    // Calcular la dirección física: marco de página * tamaño de página + offset
    int physical_address = (entry.page_frame * PAGE_SIZE) + offset;
    return physical_address;
}

/**
 * Función: main
 * Descripción: Ejecuta la simulación solicitando al usuario una dirección virtual y calculando
 *              su dirección física si es posible. También calcula el tamaño del espacio de
 *              direcciones virtuales.
 */
int main() {
    uint32_t virtual_address;

    // a) Formato de la dirección virtual
    printf("Formato de la dirección virtual:\n");
    printf(" - Número de página: 8 bits (bits 12 a 19 de la dirección)\n");
    printf(" - Offset dentro de la página: 12 bits (bits 0 a 11 de la dirección)\n");

    // Solicitar al usuario la dirección virtual en hexadecimal
    printf("\nIngrese una dirección virtual (en hexadecimal, hasta 32 bits): ");
    scanf("%x", &virtual_address);

    // b) Calcular la dirección física correspondiente
    int physical_address = get_physical_address(virtual_address);
    if (physical_address != -1) {
        printf("Dirección física correspondiente: 0x%X\n", physical_address);
    }

    // c) Tamaño del espacio de direcciones virtuales
    uint64_t virtual_memory_size = (uint64_t)1 << VIRTUAL_ADDRESS_BITS;
    printf("El tamaño del espacio de direcciones virtuales es: %llu bytes (%.2f GB)\n", virtual_memory_size, virtual_memory_size / (double)(1 << 30));

    return 0;
}
