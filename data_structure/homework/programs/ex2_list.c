#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

/*
   Imprimir la lista
*/

static void print_list(const DList *list) {
    DListNode *node;
    char *data;
    int i;

    fprintf(stdout, "El tamaño de la lista es %d\n", dlist_size(list));

    i = 0;
    node = dlist_head(list);

    while (1) {
        data = dlist_data(node);
        fprintf(stdout, "lista.nodo[%03d] = %c, %14p <- %p -> %p \n",
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
    char *data;
    int i;

    // Inicializar la lista doblemente enlazada
    dlist_init(&list, free);

    // Insertar las 27 letras del alfabeto inglés (A–Z + '_')
    for (i = 26; i >= 0; i--) {
        if ((data = (char *)malloc(sizeof(char))) == NULL)
            return 1;

        if (i == 26)
            *data = '_';   // carácter adicional para llegar a 27
        else
            *data = 'A' + i;

        if (dlist_ins_prev(&list, dlist_head(&list), data) != 0)
            return 1;
    }

    print_list(&list);

    // Ejemplo: eliminar un nodo después del quinto elemento
    node = dlist_head(&list);
    for (i = 0; i < 5; i++)
        node = dlist_next(node);

    data = dlist_data(node);
    fprintf(stdout, "\nEliminando un nodo después del que contiene %c\n", *data);

    if (dlist_remove(&list, node, (void**)&data) != 0)
        return 1;

    free(data); // liberar el dato eliminado
    print_list(&list);

    // Insertar un carácter especial al final de la lista
    fprintf(stdout, "\nInsertando '@' al final de la lista\n");
    if ((data = (char *)malloc(sizeof(char))) == NULL)
        return 1;
    *data = '@';
    if (dlist_ins_next(&list, dlist_tail(&list), data) != 0)
        return 1;
    print_list(&list);

    // Eliminar el último nodo de la lista
    fprintf(stdout, "\nEliminando un nodo al final de la lista\n");
    if (dlist_remove(&list, dlist_tail(&list), (void**)&data) != 0)
        return 1;
    free(data);
    print_list(&list);

    // Insertar antes del primer nodo (cabeza)
    fprintf(stdout, "\nInsertando '#' antes del primer nodo\n");
    if ((data = (char *)malloc(sizeof(char))) == NULL)
        return 1;
    *data = '#';
    if (dlist_ins_prev(&list, dlist_head(&list), data) != 0)
        return 1;
    print_list(&list);

    // Eliminar el primer nodo de la lista
    fprintf(stdout, "\nEliminando el primer nodo de la lista\n");
    if (dlist_remove(&list, dlist_head(&list), (void**)&data) != 0)
        return 1;
    free(data);
    print_list(&list);

    // Destruir la lista y liberar memoria
    fprintf(stdout, "\nDestruyendo la lista completa\n");
    dlist_destroy(&list);

    return 0;
}

