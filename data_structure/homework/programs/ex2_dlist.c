#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

/*
   Imprimir la lista
*/

static void print_list(const DList *list) {
    DListNode *node;
    int *data, i;

    fprintf(stdout, "El tamaño de la lista es %d\n", dlist_size(list));

    i = 0;
    node = dlist_head(list);

    while (1) {
        data = dlist_data(node);
        fprintf(stdout, "lista.nodo[%03d] = %03d, %14p <- %p -> %p \n",
                i, *data, node->prev, node, node->next);

        i++;

        if (dlist_is_tail(node))
            break;
        else
            node = dlist_next(node);
    }

   return;
}

int main(int argc, char **argv) {
    DList list;
    DListNode *node;
    int *data, i;

    // Verificar que se pasen valores desde la línea de comandos
    if (argc < 2) {
        fprintf(stderr, "Uso: %s valor1 valor2 valor3 ...\n", argv[0]);
        return 1;
    }

    // Inicializar la lista doblemente ligada
    dlist_init(&list, free);

    // Insertar los valores recibidos en la línea de comandos
    for (i = 1; i < argc; i++) {
        if ((data = (int *)malloc(sizeof(int))) == NULL)
            return 1;

        *data = atoi(argv[i]); // convertir cada argumento a entero

        // Insertar al final de la lista
        if (dlist_size(&list) == 0) {
            if (dlist_ins_prev(&list, NULL, data) != 0)
                return 1;
        } else {
            if (dlist_ins_next(&list, dlist_tail(&list), data) != 0)
                return 1;
        }
    }

    // Imprimir la lista con los valores ingresados
    print_list(&list);

    // Destruir la lista y liberar memoria
    fprintf(stdout, "\nDestruyendo la lista\n");
    dlist_destroy(&list);

    return 0;
}

