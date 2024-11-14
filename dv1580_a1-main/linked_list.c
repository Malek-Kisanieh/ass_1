#include <stdio.h>
#include <stdint.h>
#include "memory_manager.h"

// Define a structure named Node that we can use during the implementation:
typedef struct Node {
    uint16_t data;       // Stores the data as an unsigned 16_bit integer
    struct Node* next;    //. A pointer to the next node in the list
} Node;


// Initializes a linked list and the custom memory manager.
// Parameters:
// - head: Pointer to the head pointer of the linked list.
// - size: Size of the memory pool to be initialized.

// The function sets up the list and prepares it for operations
void list_init(Node** head, size_t size) {
    *head = NULL;
    mem_init(size);
}

// Funktion för att infoga en ny nod i listan
void list_insert(Node** head, uint16_t data) {
    // Skapar en ny nod och allokerar minne för den med hjälp av den anpassade minneshanteraren
    Node* new_node = (Node*) mem_alloc(sizeof(Node));
    
    // Kontrollera om minnesallokeringen lyckades
    if (!new_node) {
        printf("Minnesallokering misslyckades\n");  // Om allokeringen misslyckas, skriv ut felmeddelande
        return;  // Avslutar funktionen
    }
    
    // Tilldela det specifika datavärdet till den nya noden
    new_node->data = data;
    // Sätt nästa pekare till NULL, eftersom den ska vara den sista noden i listan
    new_node->next = NULL;

    // Kontrollera om listan är tom
    if (*head == NULL) {
        *head = new_node;  // Om listan är tom, sätt den nya noden som första nod (head)
    } else {
        // Om listan inte är tom, hitta sista noden
        Node* current = *head;  // Starta från början av listan
        while (current->next != NULL) {  // Loopa tills vi når sista noden
            current = current->next;  // Flytta pekaren till nästa nod
        }
        // Sätt sista nodens 'next' pekare till den nya noden
        current->next = new_node;
    }
}


// Infogar en ny nod direkt efter en given nod.
// Parametrar:
// - prev_node: Noden efter vilken den nya noden ska infogas.
// - data: Datan som ska infogas i den nya noden.
// Felhantering:
// - Skriver ut ett felmeddelande om den föregående noden är NULL.
// - Skriver ut ett felmeddelande om minnesallokeringen misslyckas.
void list_insert_after(Node* prev_node, uint16_t data) {
    // Kontrollera om föregående nod är NULL
    if (prev_node == NULL) {
        printf("Den föregående noden får inte vara NULL\n");  // Felmeddelande om prev_node är NULL
        return;  // Avslutar funktionen eftersom det är ett ogiltigt tillstånd
    }

    // Skapa en ny nod och allokera minne för den
    Node* new_node = (Node*) mem_alloc(sizeof(Node));
    // Kontrollera om minnesallokeringen lyckades
    if (!new_node) {
        printf("Minnesallokering misslyckades\n");  // Felmeddelande om minnesallokeringen misslyckades
        return;  // Avslutar funktionen
    }

    // Tilldela det angivna värdet till den nya nodens datafält
    new_node->data = data;

    // Länka den nya noden till nästa nod i listan
    new_node->next = prev_node->next;

    // Uppdatera den föregående nodens pekare så att den pekar på den nya noden
    prev_node->next = new_node;
}

// Infogar en ny nod före en given nod.
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
// - next_node: Noden före vilken den nya noden ska infogas.
// - data: Datan som ska infogas i den nya noden.

void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    // Kontrollera om next_node är NULL
    if (next_node == NULL) {
        printf("Next node får inte vara NULL\n");  // Felmeddelande om next_node är NULL
        return;  // Avslutar funktionen
    }

    // Skapa en ny nod och allokera minne för den
    Node* new_node = (Node*) mem_alloc(sizeof(Node));
    // Kontrollera om minnesallokeringen lyckades
    if (!new_node) {
        printf("Minnesallokering misslyckades\n");  // Felmeddelande om minnesallokeringen misslyckades
        return;  // Avslutar funktionen
    }
    // Tilldela värdet till den nya nodens datafält
    new_node->data = data;

    // Om next_node är huvudnoden, infoga den nya noden som ny huvudnod
    if (*head == next_node) {
        new_node->next = *head;  // Länka nya noden till den gamla huvudnoden
        *head = new_node;  // Uppdatera huvudpekaren till den nya noden
        return;  // Avslutar funktionen
    }

    // Sök efter noden som pekar på next_node
    Node* current = *head;
    while (current != NULL && current->next != next_node) {
        current = current->next;  // Gå till nästa nod i listan
    }

    // Om next_node inte hittas i listan
    if (current == NULL) {
        printf("Den angivna nästa noden finns inte i listan\n");  // Felmeddelande om next_node inte hittades
        mem_free(new_node);  // Frigör minnet som tilldelades för den nya noden
        return;  // Avslutar funktionen
    }

    // Länka den nya noden till next_node och sätt current's next till den nya noden
    new_node->next = next_node;  // Den nya noden pekar på next_node
    current->next = new_node;  // Sätt current's next till den nya noden
}


// Tar bort den första noden med angiven data.
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
// - data: Datan för noden som ska tas bort.
// Felhantering:
// - Skriver ut ett felmeddelande om listan är tom.
// - Skriver ut ett felmeddelande om datan inte hittas i listan.
void list_delete(Node** head, uint16_t data) {
    if (*head == NULL) {
        printf("Listan är tom\n");  // Felmeddelande om listan är tom
        return;
    }

    Node* current = *head;  // Pekare till den aktuella noden
    Node* previous = NULL;  // Pekare till den föregående noden

    // Leta efter noden med den specifika datan
    while (current != NULL && current->data != data) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Data hittades inte i listan\n");  // Felmeddelande om datan inte hittas
        return;
    }

    // Om noden är den första i listan
    if (previous == NULL) {
        *head = current->next;  // Uppdatera huvudpekaren
    } else {
        previous->next = current->next;  // Hoppa över den aktuella noden
    }

    mem_free(current);  // Frigör minnet för den borttagna noden
}

// Söker efter en nod med angiven data.
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
// - data: Datan som ska sökas efter.
// Returnerar:
// - En pekare till noden som innehåller datan om den hittas, annars NULL.
Node* list_search(Node** head, uint16_t data) {
    Node* current = *head;  // Pekare till den aktuella noden
    while (current != NULL) {
        if (current->data == data) {
            return current;  // Returnerar pekare till noden om datan hittas
        }
        current = current->next;  // Gå till nästa nod i listan
    }
    return NULL;  // Returnerar NULL om datan inte hittas
}

// Visar alla element i listan.
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
void list_display(Node** head) {
    Node* current = *head;  // Pekare till den aktuella noden
    printf("[");
    while (current != NULL) {
        printf("%u", current->data);  // Skriv ut datan för den aktuella noden
        if (current->next != NULL) {
            printf(", ");  // Skriv ut komma om det finns fler noder
        }
        current = current->next;
    }
    printf("]");
}

// Visar element mellan två noder (inklusive).
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
// - start_node: Startnoden (inklusive). Om NULL börjar från huvudet.
// - end_node: Slutnoden (inklusive). Om NULL går till slutet.
void list_display_range(Node** head, Node* start_node, Node* end_node) {
    Node* current = start_node ? start_node : *head;  // Starta från start_node eller huvudnoden

    printf("[");
    while (current != NULL && (end_node == NULL || current != end_node->next)) {
        printf("%d", current->data);  // Skriv ut datan för den aktuella noden
        if (current->next != NULL && current != end_node) {
            printf(", ");  // Skriv ut komma om det finns fler noder
        }
        current = current->next;  // Gå till nästa nod
    }
    printf("]");
}

// Räknar antalet noder i listan.
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
// Returnerar:
// - Det totala antalet noder i listan.
int list_count_nodes(Node** head) {
    int count = 0;  // Räknare för noder
    Node* current = *head;  // Pekare till den aktuella noden
    while (current != NULL) {
        count++;  // Öka räknaren för varje nod
        current = current->next;  // Gå till nästa nod
    }
    return count;  // Returnerar det totala antalet noder
}

// Frigör alla noder i listan och avallokerar minneshanteraren.
// Parametrar:
// - head: Pekare till pekaren som håller listans första nod.
void list_cleanup(Node** head) {
    Node* current = *head;  // Pekare till den aktuella noden
    while (current != NULL) {
        Node* next_node = current->next;  // Pekare till nästa nod
        mem_free(current);  // Frigör minnet för den aktuella noden
        current = next_node;  // Gå till nästa nod
    }
    *head = NULL;  // Sätter huvudpekaren till NULL för att markera listan som tom
    mem_deinit();  // Avslutar minneshanteraren
}
