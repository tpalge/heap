#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "heap.h"

#define TAMANIO_INICIAL 35
#define CANT_A_MODIFICAR 2

/* Integrantes del grupo */

/* Rodrigo Aparicio */
/* Padrón: 98967    */

/* Santiago Bozzo   */
/* Padrón: 98772    */

/* Definición del struct heap.
 */
 
struct heap{
	size_t cant;
	void** datos;
	size_t tamanio_arreglo;
	cmp_func_t funcion_comparacion;
};


/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().*/
heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap)
		return NULL;
	
	heap->datos = malloc(sizeof(void*) * TAMANIO_INICIAL);
	if(!heap->datos){
		free(heap);
		return NULL;		
	}
	
	heap->tamanio_arreglo = TAMANIO_INICIAL;	
	heap->cant = 0;
	heap->funcion_comparacion = cmp;
	
	return heap;
}

//Intercambia dos valores en un arreglo.
void swap (void** datos, size_t posicion, size_t pos_padre){
	void* auxiliar = datos[pos_padre];
	datos[pos_padre] = datos[posicion];
	datos[posicion] = auxiliar;
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap){
	return heap->cant;	
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap){
	return (heap->cant == 0);	
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL. */
void *heap_ver_max(const heap_t *heap){
	if (!heap_esta_vacio(heap))	
		return (heap->datos[0]);	
		
	return NULL;
}

//Dado un arreglo y una posicion le aplica upheap al elemento en esa posicion
//en el arreglo.
void upheap(void** datos, void* dato, size_t posicion, cmp_func_t cmp){
	if (posicion == 0)
		return;
		
	size_t pos_padre = (posicion-1) / 2;
	
	int resultado = cmp(datos[posicion],datos[pos_padre]);
	
	if (resultado > 0){ // el primer paramétro es más grande
		swap(datos, posicion, pos_padre);
		upheap(datos, dato, pos_padre, cmp);		
	}	
}

/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap. */
bool heap_encolar(heap_t *heap, void *elem){
	if (!elem)
		return false;
	
	if (heap->cant == heap->tamanio_arreglo){ // redimensión para arriba
		void** datos_nuevo = realloc(heap->datos, sizeof(void*) * heap->tamanio_arreglo * CANT_A_MODIFICAR);
		if (!datos_nuevo)
			return false;
		heap->datos = datos_nuevo;
		heap->tamanio_arreglo = heap->tamanio_arreglo * CANT_A_MODIFICAR;		
	}
	
	heap->datos[heap->cant] = elem;
	upheap(heap->datos, elem, heap->cant, heap->funcion_comparacion);	
	
	heap->cant++;
	
	return true;	
}

//Dado un arreglo y una posicion aplica downheap al elemento en esa posicion
//en el arreglo.
void downheap(void **datos, size_t cant, size_t pos, cmp_func_t cmp){
	if(pos >= cant){
		return;
	}
	size_t pos_h_izq = pos*2+1;
	size_t pos_h_der = pos*2+2;
	size_t pos_mayor = pos;
	if((pos_h_izq < cant) && (cmp(datos[pos], datos[pos_h_izq]) < 0)){
		pos_mayor = pos_h_izq;
	}
	if((pos_h_der < cant) && (cmp(datos[pos_mayor], datos[pos_h_der]) < 0)){
		pos_mayor = pos_h_der;
	}
	if(pos_mayor != pos){
		swap(datos, pos, pos_mayor);
		downheap(datos, cant, pos_mayor, cmp);
	}
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap. */
void *heap_desencolar(heap_t *heap){
	if(heap_esta_vacio(heap)){
		return NULL;
	}
	void *dato = heap->datos[0];
	heap->datos[0] = heap->datos[heap->cant-1];
	heap->cant--;
	downheap(heap->datos, heap->cant, 0, heap->funcion_comparacion);
	
	//redimensionar para abajo
	if((heap->cant == heap->tamanio_arreglo/4) && (heap->cant > TAMANIO_INICIAL)){
		void** datos_nuevo = realloc(heap->datos, (heap->tamanio_arreglo/2) * sizeof(void*));
		if(datos_nuevo){
			heap->datos = datos_nuevo;
			heap->tamanio_arreglo = heap->tamanio_arreglo/2;
		}
	}
	
	return dato;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){
		for (size_t i = 0; i < heap->cant; i++){
			destruir_elemento(heap->datos[i]);			
		}		
	}
	
	free(heap->datos);
	free(heap);	
}

//Dado un arreglo le aplica heapify dejandolo ordenado en forma de heap.
void heapify(void** arreglo, int posicion, cmp_func_t cmp, size_t cant){ 
	if (posicion < 0)
		return;
		
	size_t pos_hijo_izq = 2 * posicion + 1;
	size_t pos_hijo_der = 2 * posicion + 2;
	
	if ((pos_hijo_izq < cant && arreglo[posicion] < arreglo[pos_hijo_izq]) || (pos_hijo_der < cant && arreglo[posicion] < arreglo[pos_hijo_der]))
		downheap(arreglo, cant, posicion, cmp);
	
	posicion--;
	heapify(arreglo, posicion, cmp, cant);
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place". */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	if (cant == 0)
		return; // sino pincha en el for donde ordeno ya que cant - 1 no es -1 porque cant es size_t
	
	int ult_pos_no_hoja = (int)cant / 2 - 1;
	heapify(elementos, ult_pos_no_hoja, cmp, cant);
	
	size_t ult_pos_heap = cant - 1;
	
	for (size_t i = 0; i < cant - 1; i++){
		swap(elementos, 0, ult_pos_heap);
		downheap(elementos, cant - (i + 1), 0, cmp);		
		ult_pos_heap--;
	}			
}

/* Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n). */
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = heap_crear(cmp);
	if (!heap)
		return NULL;
	if(n == 0){return heap;}
	int pos = (int)n/2-1;
	heapify(arreglo, pos, cmp, n);
	
	heap_encolar(heap, arreglo[0]);
	for(size_t i = 0; i < n; i++){
		if(i*2+1 < n){
			heap_encolar(heap, arreglo[i*2+1]);
		}
		if(i*2+2 < n){
			heap_encolar(heap, arreglo[i*2+2]);
		}
	}
	return heap;
}
