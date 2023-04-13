#include "set_utils.h"
#include <stdio.h>

Pointer set_find_eq_or_greater(Set set, Pointer value) {

    Pointer r_value = set_find(set, value);
    if (r_value == NULL) {
       set_insert(set, value);
       SetNode node = set_find_node(set, value);
       node = set_next(set, node);
       r_value = set_node_value(set, node);
       set_remove(set, value);
    }

    return r_value;
}

Pointer set_find_eq_or_smaller(Set set, Pointer value) {

    Pointer r_value = set_find(set, value);
    if (r_value == NULL) {
       set_insert(set, value);
       SetNode node = set_find_node(set, value);
       node = set_previous(set, node);
       r_value = set_node_value(set, node);
       set_remove(set, value);
    }

    return r_value;
}