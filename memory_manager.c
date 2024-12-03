#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

typedef struct MemBlock {
    size_t block_size;           
    int is_available;            
    struct MemBlock* next_block; 
    void* data_ptr;              
} MemBlock;


void* pool_start = NULL;       
MemBlock* pool_head = NULL;    // Pekare till det första minnesblocket i den länkade listan
size_t total_pool_size = 0;    // Totala storleken på minnespoolen i byte

// Funktion för att initiera minnespoolen med en given storlek
// Parametrar:
// - pool_size: Den totala storleken som ska allokeras för poolen
// Felhantering:
// - Skriver ut ett felmeddelande och avslutar programmet om minnesallokeringen misslyckas
void mem_init(size_t pool_size) {
    // Allokera minne för hela minnespoolen
    pool_start = malloc(pool_size);
    if (!pool_start) {
        perror("Misslyckades med att allokera minnespool");
        exit(EXIT_FAILURE);
    }

    total_pool_size = pool_size;

    // Skapa det första minnesblocket som täcker hela poolen
    pool_head = (MemBlock*)malloc(sizeof(MemBlock));
    if (!pool_head) {
        perror("Misslyckades med att skapa blockmetadata");
        free(pool_start);
        exit(EXIT_FAILURE);
    }

    // Initialisera det första blocket som ledigt och täcker hela poolen
    pool_head->block_size = pool_size;
    pool_head->is_available = 1;        // Markera blocket som tillgängligt
    pool_head->data_ptr = pool_start;   // Peka på startadressen av minnespoolen
    pool_head->next_block = NULL;       // Inget nästa block än
}

// Funktion för att allokera ett minnesblock av specificerad storlek från poolen
// Parametrar:
// - size: Antalet byte som ska allokeras.
// Returnerar:
// - Pekare till det allokerade minnesblocket ggom lyckat, annars NULL om inget lämpligt block finns
void* mem_alloc(size_t size) {
    MemBlock* current = pool_head;

    // Traversera listan för att hitta ett ledigt block med tillräcklig storlek
    while (current != NULL) {
        if (current->is_available && current->block_size >= size) {
            if (current->block_size > size) {
                // Om blocket är större än behövligt, dela upp det i två block
                MemBlock* new_block = (MemBlock*)malloc(sizeof(MemBlock));
                if (!new_block) {
                    perror("Misslyckades med att skapa nytt blockmetadata");
                    return NULL;
                }

                // Initiera det nya blocket med den återstående storleken
                new_block->block_size = current->block_size - size;
                new_block->is_available = 1; // Nya blocket är tillgängligt
                new_block->data_ptr = (char*)current->data_ptr + size; // Justera datapekaren
                new_block->next_block = current->next_block; // Länka till nästa block

                // Uppdatera det aktuella blocket till den begärda storleken och markera det som upptaget
                current->block_size = size;
                current->is_available = 0; // Markera som upptaget
                current->next_block = new_block; // Länka till det nya blocket
            } else {
                // Om blockets storlek exakt matchar den begärda storleken, markera det som upptaget
                current->is_available = 0;
            }

            // Returnera pekaren till det allokerade dataområdet
            return current->data_ptr;
        }
        current = current->next_block; // Gå vidare till nästa block i listan
    }

    // Returnera NULL om inget lämpligt block hittades
    return NULL;
}

// Funktion för att frigöra ett tidigare allokerat minnesblock i poolen
// Parametrar:
// - ptr: Pekare till det minnesblock som ska frigöras
// Felhantering:
// - Ignorerar NULL-pekare och varnar om blocket redan är fritt eller inte hittas.
void mem_free(void* ptr) {
    if (!ptr) {
        fprintf(stderr, "Varning: Försökte frigöra en NULL-pekare.\n");
        return;
    }

    MemBlock* current = pool_head;
    while (current != NULL) {
        if (current->data_ptr == ptr) {
            if (current->is_available) {
                fprintf(stderr, "Varning: Blocket vid %p är redan fritt.\n", ptr);
                return;
            }

            // Markera blocket som ledigt
            current->is_available = 1;

            // Försök att slå samman med nästa block om det också är ledigt, för att undvika fragmentering
            MemBlock* next_block = current->next_block;
            while (next_block != NULL && next_block->is_available) {
                current->block_size += next_block->block_size; // Öka storleken på det nuvarande blocket
                current->next_block = next_block->next_block; // Hoppa över nästa block i listan
                free(next_block); // Frigör metadata för nästa block
                next_block = current->next_block; // Uppdatera pekaren till nästa block
            }

            return;
        }
        current = current->next_block; // Gå vidare till nästa block i listan
    }

    // Om pekaren inte hittas i poolen, ge en varning
    fprintf(stderr, "Varning: Pekaren %p var inte allokerad från denna pool.\n", ptr);
}

// Funktion för att ändra storleken på ett allokerat minnesblock i poolen
// Parametrar:
// - ptr: Pekare till det minnesblock som ska ändras
// - size: Den nya storleken i byte
// Returnerar:
// - Pekare till det omallokerade minnesblocket eller NULL om ändringen misslyckas
void* mem_resize(void* ptr, size_t size) {
    if (!ptr) return mem_alloc(size); // Om pekaren är NULL, allokera nytt minne

    MemBlock* block = pool_head;
    while (block != NULL) {
        if (block->data_ptr == ptr) {
            if (block->block_size >= size) {
                return ptr; // Nuvarande block är tillräckligt stort, returnera samma pekare
            } else {
                // Allokera ett nytt block med den önskade storleken
                void* new_ptr = mem_alloc(size);
                if (new_ptr) {
                    // Kopiera data från det gamla blocket till det nya
                    memcpy(new_ptr, ptr, block->block_size);
                    // Frigör det gamla blocket
                    mem_free(ptr);
                }
                return new_ptr; // Returnera pekaren till det nya blocket eller NULL om allokering misslyckades
            }
        }
        block = block->next_block; // Gå vidare till nästa block i listan
    }

    // Om pekaren inte hittas i poolen, ge en varning
    fprintf(stderr, "Varning: Ändring av storlek misslyckades, pekaren %p hittades inte.\n", ptr);
    return NULL;
}

// Funktion för att avinitiera minnespoolen och frigöra alla resurser
void mem_deinit() {
    free(pool_start); // Frigör hela minnespoolen
    pool_start = NULL; // Sätt pool_start till NULL för att undvika hängande pekare

    MemBlock* current = pool_head;
    while (current != NULL) {
        MemBlock* next = current->next_block; // Spara nästa block
        free(current); // Frigör nuvarande blockmetadata
        current = next; // Gå vidare till nästa block
    }

    pool_head = NULL;        // Sätt pool_head till NULL för att indikera att listan är tom
    total_pool_size = 0;     // Återställ den totala poolstorleken till 0
}
