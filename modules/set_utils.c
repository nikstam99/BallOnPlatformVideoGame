#include "set_utils.h"
#include <stdio.h>

// Συνάρτηση που επιστρέφει την τιμή value ενός set ή
// το ακριβώς επόμενο στοιχειο αν αυτό δεν βρεθεί.
Pointer set_find_eq_or_greater(Set set, Pointer value) {

    // Ψάχνουμε στο set το value.
    Pointer r_value = set_find(set, value);

    // Αν το value δεν βρεθεί το τοποθετούμε εμείς στο set.
    if (r_value == NULL) {
       set_insert(set, value);

       // Βρίσκουμε τον κόμβο του value.
       SetNode node = set_find_node(set, value);

       // Διαλέγουμε τον επόμενο κόμβο.
       node = set_next(set, node);

       // Παίρνουμε το value του επόμενου κόμβου.
       r_value = set_node_value(set, node);

       // Αφαιρούμε από το set το αρχικό value που του τοποθετήσαμε.
       set_remove(set, value);
    }

    // Επιστρέφουμε το value.
    return r_value;
}

// Συνάρτηση που επιστρέφει την τιμή value ενός set ή
// το ακριβώς προηγούμενο στοιχειο αν αυτό δεν βρεθεί.
Pointer set_find_eq_or_smaller(Set set, Pointer value) {

    // Ψάχνουμε στο set το value.
    Pointer r_value = set_find(set, value);

    // Αν το value δεν βρεθεί το τοποθετούμε εμείς στο set.
    if (r_value == NULL) {
       set_insert(set, value);

       // Βρίσκουμε τον κόμβο του value.
       SetNode node = set_find_node(set, value);

       // Διαλέγουμε τον επόμενο κόμβο.
       node = set_previous(set, node);

       // Παίρνουμε το value του επόμενου κόμβου.
       r_value = set_node_value(set, node);

       // Αφαιρούμε από το set το αρχικό value που του τοποθετήσαμε.
       set_remove(set, value);
    }

    // Επιστρέφουμε το value.
    return r_value;
}