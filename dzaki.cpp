#include "poker.h"

char type_assign (int jenis) {
    char tipe;
    switch (jenis) {
    case 1:
        return tipe = 'A';
    case 2:
        return tipe = 'B';
    case 3:
        return tipe = 'C';
    case 4:
        return tipe = 'D';
    }
}

void alloc_card (int nomor, int nilai, int jenis, nodeKartu **newNode) { 
    char tipe;
    *newNode = (nodeKartu *) malloc(sizeof(nodeKartu));
    if (newNode == NULL)
    {
        printf("Memory Sudah Full");
    }
    else
    {
        tipe = type_assign(jenis);
        (*newNode)->nomorKartu = nomor;
        (*newNode)->nilaiKartu = nilai;
        (*newNode)->tipeKartu = tipe;
        (*newNode)->next = NULL;
    }
}

void fill_the_card(pointKartu *dekLL) {
    nodeKartu *newNode;
    int nomor = 1;
    int nilai = 3;
    int jenis = 1;

    while (jenis <= 4) {
        while (nilai <= 15) {
            alloc_card(nomor, nilai, jenis, &newNode);
            if (dekLL->head == NULL) {
                dekLL->head = newNode;
            } else {
                dekLL->tail->next = newNode;
            }
            dekLL->tail = newNode;
            nomor++;
            nilai++;
        }
        nilai = 3;
        jenis++;
    }
}

int count_card(nodeKartu *head) {
    int count = 0;
    nodeKartu *temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

nodeKartu* get_node_at(nodeKartu *head, int index) {
    nodeKartu *current = head;
    int count = 0;
    while (current != NULL && count < index) {
        current = current->next;
        count++;
    }
    return current;
}

void shuffle_deck(pointKartu *dekLL) {
    int count = count_card(dekLL->head);
    if (count < 2) return;
    srand(time(NULL));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        if (i != j) {
            // Tukar node ke-i dan node ke-j
            nodeKartu *node_i = get_node_at(dekLL->head, i);
            nodeKartu *node_j = get_node_at(dekLL->head, j);

            int temp_nilai = node_i->nilaiKartu;
            int temp_jenis = node_i->tipeKartu;
            node_i->nilaiKartu = node_j->nilaiKartu;
            node_i->tipeKartu = node_j->tipeKartu;
            node_j->nilaiKartu = temp_nilai;
            node_j->tipeKartu = temp_jenis;
        }
    }
}

void alloc_player (nodePemain **newNode, char nama[]) { 
    *newNode = (nodePemain *) malloc(sizeof(nodePemain));
    if (newNode == NULL)
    {
        printf("Memory Sudah Full");
    }
    else
    {
        strcpy((*newNode)->nama, nama);
        (*newNode)->kartu = {NULL, NULL};
        (*newNode)->pemain = NULL;
    }
}

void create_player (nodePemain **temp, char nama[]) {
    char *computer_names[] = {"Com1", "Com2", "Com3"};
    nodePemain *newNode, *ujung;
    ujung = *temp;

    // Untuk Player
    alloc_player(&newNode, nama);
    *temp = newNode;
    ujung = newNode;

    // Untuk Computer
    for (int i = 0; i < 3; i++) {
        alloc_player(&newNode, computer_names[i]);
        (*temp)->pemain = newNode;
        *temp = newNode;
    }

    (*temp)->pemain = ujung;
    *temp = ujung;
}

void display_node (nodeKartu *head) { 
    nodeKartu *temp = head;
    printf("Linked List = ");
    while (temp != NULL) {
        printf("%d | %c | %d -> ", temp->nomorKartu, temp->tipeKartu, temp->nilaiKartu);
        temp = temp->next;
    }
    printf("NULL\n");
}

void print_players(nodePemain *head) {
    if (head == NULL) return;
    nodePemain *current = head;
    do {
        printf("\nPemain : %s\n", current->nama);
        display_node((current)->kartu.head);
        current = current->pemain;
    } while (current != head);
}

void insert_last(pointKartu *llKartu, nodeKartu *newNode) {
    if (llKartu->tail != NULL) {
        llKartu->tail->next = newNode;
    }
    llKartu->tail = newNode;
    newNode->next = NULL;

    if (llKartu->head == NULL) {
        llKartu->head = newNode;
    }
}

void insert_order(pointKartu *llKartu, nodeKartu *newNode) {
    if (llKartu->head == NULL) {
        // Linked list kosong
        llKartu->head = newNode;
        llKartu->tail = newNode;
        newNode->next = NULL;
    } else if (newNode->nilaiKartu <= llKartu->head->nilaiKartu) {
        // Insert di depan
        newNode->next = llKartu->head;
        llKartu->head = newNode;
    } else {
        // Insert di tengah atau belakang
        nodeKartu *current = llKartu->head;
        while (current->next != NULL && current->next->nilaiKartu < newNode->nilaiKartu) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;

        // Update tail jika newNode dimasukkan di belakang
        if (newNode->next == NULL) {
            llKartu->tail = newNode;
        }
    }
}

void assign_number (nodeKartu *head) { 
    nodeKartu *temp = head;
    int count = 1;
    while (temp != NULL) {
        temp->nomorKartu = count;
        count++;
        temp = temp->next;
    }
}

void fill_deck(nodePemain **aktif, pointKartu *dekLL) {
    nodeKartu *temp;
    int i, totalCard;

    for (i = 1; i <= 4; i++) {
        totalCard = 0;
        printf("Distribusi kartu untuk pemain %d: %s\n", i, (*aktif)->nama);
        while (totalCard < 13) {
            if (dekLL->head == NULL) {
                printf("Deck kosong!\n");
                return;
            }
            temp = dekLL->head;
            dekLL->head = dekLL->head->next;
            temp->next = NULL;

            insert_order(&((*aktif)->kartu), temp);
            // printf("Kartu %d: %d | %c\n", totalCard + 1, temp->nilaiKartu, temp->tipeKartu);
            totalCard++;
        }
        printf("Kartu pemain %s setelah distribusi:\n", (*aktif)->nama);
        assign_number((*aktif)->kartu.head);
        display_node((*aktif)->kartu.head);
        *aktif = (*aktif)->pemain;
    }
}

nodePemain *first_play(nodePemain *aktif) {
    nodePemain *winner = NULL;
    nodeKartu *temp, *prev;
    int i, totalCard;

    for (i = 1; i <= 4; i++) {
        temp = aktif->kartu.head;
        prev = NULL;
        totalCard = 0; 
        while (totalCard < 13 && temp != NULL) { 
            // printf("Nilai = %d\n", temp->nilaiKartu);

            if (temp->nilaiKartu == 3) {
                printf("Singa\n");

                // Menetapkan pemenang jika tipeKartu adalah 'D'
                if (temp->tipeKartu == 'D') {
                    winner = aktif;
                }

                // Menghapus node dari linked list
                nodeKartu* nodeToFree = temp;
                if (temp == aktif->kartu.head) {
                    aktif->kartu.head = temp->next;
                    if (aktif->kartu.head == NULL) {
                        aktif->kartu.tail = NULL;
                    }
                    temp = temp->next;
                } else if (temp == aktif->kartu.tail) {
                    prev->next = NULL;
                    aktif->kartu.tail = prev;
                    temp = NULL;
                } else {
                    if (prev != NULL) {
                        prev->next = temp->next;
                    }
                    temp = temp->next;
                }
                free(nodeToFree);
            } else {
                prev = temp;
                temp = temp->next;
            }
            totalCard++;
        }
        aktif = aktif->pemain;
    }
    return winner;
}

void display_card(nodeKartu *card) {
    printf("----------\n");
    printf("|%6d |\n", card->nomorKartu);
    printf("|%-8s|\n", card->tipeKartu);
    printf("|%6d |\n", card->nilaiKartu);
    printf("----------\n");
}

void display_player(nodeKartu *head) {
    nodeKartu *current = head;
    int count = 0;
    while (current != NULL && count < 13) {
        display_card(current);
        current = current->next;
        count++;
    }
}

void print_game_computers(nodePemain *head) {
    if (head == NULL) return;

    nodePemain *current = head;
    current = head;

    // Print header
    printf("Komputer:\n");
    do {
        printf("-------------\t");
        current = current->pemain;
    } while (current != head && strncmp(current->nama, "Dzaki", 6) != 0);
    printf("\n");

    current = head;
    do {
        printf("| %-9s |\t", current->nama);
        current = current->pemain;
    } while (current != head && strncmp(current->nama, "Dzaki", 6) != 0);
    printf("\n");

    current = head;
    do {
        if (current->kartu.head != NULL) {
            printf("| Sisa: %3d |\t", count_card(current->kartu.head));
        } else {
            printf("| No cards |\t");
        }
        current = current->pemain;
    } while (current != head && strncmp(current->nama, "Dzaki", 6) != 0);
    printf("\n");

    current = head;
    do {
        printf("-------------\t");
        current = current->pemain;
    } while (current != head && strncmp(current->nama, "Dzaki", 6) != 0);
    printf("\n");
}

void print_game_player(nodePemain *head){
    int count, i;
    if (head == NULL) return;

    nodePemain *current = head;
    while (current->pemain != head)
    {
        current = current->pemain;
    }
    nodeKartu *temp = current->kartu.head;
    do {
        printf("-------------\t");
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = current->kartu.head;
    do {
        printf("| Nomor: %2d |\t", temp->nomorKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = current->kartu.head;
    do {
        printf("| Tipe: %3c |\t", temp->tipeKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = current->kartu.head;
    do {
        printf("| Nilai: %2d |\t", temp->nilaiKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = current->kartu.head;
    do {
        printf("-------------\t");
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");
}

int main() {
    // dekLL buat 52 kartu
    // meja 
    pointKartu dekLL = {NULL, NULL};
    nodePemain *aktif = NULL;
    nodePemain *com = NULL;
    int coba;

    fill_the_card(&dekLL);
    printf("Deck sebelum diacak:\n");
    display_node(dekLL.head);

    shuffle_deck(&dekLL);
    printf("Deck setelah diacak:\n");
    display_node(dekLL.head);

    create_player(&aktif, "Dzaki");
    com = aktif->pemain;
    // print_players(aktif);
    fill_deck(&aktif, &dekLL);
    printf("Makan\n");
    aktif = first_play(aktif);
    printf("Ini Main = %s \n", aktif->nama);
    print_game_computers(com);
    // print_game_table();
    print_game_player(com);
}