/*
file               : poker.cpp
author             : Muhammad Dzaki Nurhidayat (231511083)
                     Muhammad Fachry Khairiansyah (231511084)
                     Restu Akbar (231511088)
program description: Program tugas besar mata kuliah Struktur Data dan Algoritma (praktik).
                     Program Games Poker Tongkrongan.
*/

#include "poker.h"

char type_assign(int jenis)
{
    char tipe;
    switch (jenis)
    {
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

void alloc_card(int nomor, int nilai, int jenis, nodeKartu **newNode)
{
    char tipe;
    *newNode = (nodeKartu *)malloc(sizeof(nodeKartu));
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

void fill_the_card(pointKartu *dekLL)
{
    nodeKartu *newNode;
    int nomor = 1;
    int nilai = 3;
    int jenis = 1;

    while (jenis <= 4)
    {
        while (nilai <= 15)
        {
            alloc_card(nomor, nilai, jenis, &newNode);
            if (dekLL->head == NULL)
            {
                dekLL->head = newNode;
            }
            else
            {
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

int count_card(nodeKartu *head)
{
    int count = 0;
    nodeKartu *temp = head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

nodeKartu *get_node_at(nodeKartu *head, int index)
{
    nodeKartu *current = head;
    int count = 0;
    while (current != NULL && count < index)
    {
        current = current->next;
        count++;
    }
    return current;
}

void shuffle_deck(pointKartu *dekLL)
{
    int count = count_card(dekLL->head);
    if (count < 2)
        return;
    srand(time(NULL));
    for (int i = count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        if (i != j)
        {
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

void alloc_player(nodePemain **newNode, char nama[])
{
    *newNode = (nodePemain *)malloc(sizeof(nodePemain));
    if (newNode == NULL)
    {
        printf("Memory Sudah Full");
    }
    else
    {
        strcpy((*newNode)->nama, nama);
        (*newNode)->kartu = {NULL, NULL};
        (*newNode)->next = NULL;
    }
}

void create_player(nodePemain **temp, char nama[])
{
    char *computer_names[] = {"Com1", "Com2", "Com3"};
    nodePemain *newNode, *ujung;
    ujung = *temp;

    // Untuk Player
    alloc_player(&newNode, nama);
    *temp = newNode;
    ujung = newNode;

    // Untuk Computer
    for (int i = 0; i < 3; i++)
    {
        alloc_player(&newNode, computer_names[i]);
        (*temp)->next = newNode;
        *temp = newNode;
    }

    (*temp)->next = ujung;
    *temp = ujung;
}

void display_node(nodeKartu *head)
{
    nodeKartu *temp = head;
    printf("Linked List = ");
    while (temp != NULL)
    {
        printf("%d | %c | %d -> ", temp->nomorKartu, temp->tipeKartu, temp->nilaiKartu);
        temp = temp->next;
    }
    printf("NULL\n");
}

void print_players(nodePemain *head)
{
    if (head == NULL)
        return;
    nodePemain *current = head;
    do
    {
        printf("\nPemain : %s\n", current->nama);
        // display_node((current)->kartu.head);
        current = current->next;
    } while (current != head);
}

void insert_order(pointKartu *llKartu, nodeKartu *newNode)
{
    // puts("TESSSS ORDER 1");
    if (llKartu->head == NULL)
    {
        llKartu->head = newNode;
        llKartu->tail = newNode;
        newNode->next = NULL;
        // puts("TESSSS ORDER 1");
    }
    else if (newNode->nilaiKartu <= llKartu->head->nilaiKartu)
    {
        newNode->next = llKartu->head;
        llKartu->head = newNode;
    }
    else
    {
        nodeKartu *current = llKartu->head;
        while (current->next != NULL && current->next->nilaiKartu < newNode->nilaiKartu)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;

        if (newNode->next == NULL)
        {
            llKartu->tail = newNode;
        }
    }
}

void assign_number(nodeKartu *head)
{
    nodeKartu *temp = head;
    int count = 1;
    while (temp != NULL)
    {
        temp->nomorKartu = count;
        count++;
        temp = temp->next;
    }
}

void fill_deck(nodePemain **aktif, pointKartu *dekLL)
{
    nodeKartu *temp;
    int i, totalCard;

    for (i = 1; i <= 4; i++)
    {
        totalCard = 0;
        // printf("Distribusi kartu untuk pemain %d: %s\n", i, (*aktif)->nama);
        while (totalCard < 13)
        {
            if (dekLL->head == NULL)
            {
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
        // printf("Kartu pemain %s setelah distribusi:\n", (*aktif)->nama);
        assign_number((*aktif)->kartu.head);
        // display_node((*aktif)->kartu.head);
        *aktif = (*aktif)->next;
    }
}

nodePemain *first_play(nodePemain *aktif)
{
    nodePemain *winner = NULL;
    nodeKartu *temp, *prev;
    int i, totalCard;

    for (i = 1; i <= 4; i++)
    {
        temp = aktif->kartu.head;
        prev = NULL;
        totalCard = 0;
        while (totalCard < 13 && temp != NULL)
        {
            // printf("Nilai = %d\n", temp->nilaiKartu);

            if (temp->nilaiKartu == 3)
            {
                // printf("Singa\n");

                // Menetapkan pemenang jika tipeKartu adalah 'D'
                if (temp->tipeKartu == 'D')
                {
                    winner = aktif;
                }

                // Menghapus node dari linked list
                nodeKartu *nodeToFree = temp;
                if (temp == aktif->kartu.head)
                {
                    aktif->kartu.head = temp->next;
                    if (aktif->kartu.head == NULL)
                    {
                        aktif->kartu.tail = NULL;
                    }
                    temp = temp->next;
                }
                else if (temp == aktif->kartu.tail)
                {
                    prev->next = NULL;
                    aktif->kartu.tail = prev;
                    temp = NULL;
                }
                else
                {
                    if (prev != NULL)
                    {
                        prev->next = temp->next;
                    }
                    temp = temp->next;
                }
                free(nodeToFree);
            }
            else
            {
                prev = temp;
                temp = temp->next;
            }
            totalCard++;
        }
        aktif = aktif->next;
    }
    return winner;
}

void display_card(nodeKartu *card)
{
    printf("----------\n");
    printf("|%6d |\n", card->nomorKartu);
    printf("|%-8s|\n", card->tipeKartu);
    printf("|%6d |\n", card->nilaiKartu);
    printf("----------\n");
}

void display_player(nodeKartu *head)
{
    nodeKartu *current = head;
    int count = 0;
    while (current != NULL && count < 13)
    {
        display_card(current);
        current = current->next;
        count++;
    }
}

void print_game_computers(nodePemain *head, char nama[])
{
    if (head == NULL)
        return;

    nodePemain *current = head;
    current = head;

    // Print header
    printf("Komputer:\n");
    do
    {
        printf("-------------\t");
        current = current->next;
    } while (current != head && strncmp(current->nama, nama, strlen(nama)) != 0);
    printf("\n");

    current = head;
    do
    {
        printf("| %-9s |\t", current->nama);
        current = current->next;
    } while (current != head && strncmp(current->nama, nama, strlen(nama)) != 0);
    printf("\n");

    current = head;
    do
    {
        if (current->kartu.head != NULL)
        {
            printf("| Sisa: %3d |\t", count_card(current->kartu.head));
        }
        else
        {
            printf("| No cards |\t");
        }
        current = current->next;
    } while (current != head && strncmp(current->nama, nama, strlen(nama)) != 0);
    printf("\n");

    current = head;
    do
    {
        printf("-------------\t");
        current = current->next;
    } while (current != head && strncmp(current->nama, nama, strlen(nama)) != 0);
    printf("\n");
}

void print_game_card(pointKartu *kartu)
{

    nodeKartu *temp = kartu->head;
    do
    {
        printf("-------------\t");
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("|%2d       %c |\t", temp->nilaiKartu, temp->tipeKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("|           |\t", temp->nomorKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("|           |\t", temp->nomorKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("|  No : %2d  |\t", temp->nomorKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("|           |\t", temp->nomorKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("|           |\t", temp->nomorKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("| %c      %2d |\t", temp->tipeKartu, temp->nilaiKartu);
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");

    temp = kartu->head;
    do
    {
        printf("-------------\t");
        temp = temp->next;
    } while (temp != NULL);
    printf("\n");
}

int cek_aturan(nodeMeja *dek, int *highest)
{
    if (is_it_high_card(dek->llDeck, highest))
    {
        return highCard;
    }
    else if (is_it_pair(dek->llDeck, highest))
    {
        return pair;
    }
    else if (is_it_three_of_a_kind(dek->llDeck, highest))
    {
        return threeOfaKind;
    }
    else if (is_it_straight(dek->llDeck, highest))
    {
        return straight;
    }
    else if (is_it_flush(dek->llDeck, highest))
    {
        return flush;
    }
    else if (is_it_full_house(dek->llDeck, highest))
    {
        return fullHouse;
    }
    else if (is_it_four_of_a_kind(dek->llDeck, highest))
    {
        return fourOfaKind;
    }
    else if (is_it_straight_flush(dek->llDeck, highest))
    {
        return straightFlush;
    }
    else if (is_it_royal_flush(dek->llDeck, highest))
    {
        return royalFlush;
    }
    else
    {
        return 0;
    }
}

bool is_it_high_card(pointKartu *deck, int *highest)
{
    if (deck->head == deck->tail)
    {
        *highest = deck->head->nilaiKartu;
        return true;
    }
    return false;
}

bool is_it_pair(pointKartu *deck, int *highest)
{
    if (deck->head->next == deck->tail && deck->head->nilaiKartu == deck->tail->nilaiKartu)
    {
        *highest = deck->head->nilaiKartu;
        return true;
    }
    return false;
}

bool is_it_three_of_a_kind(pointKartu *deck, int *highest)
{
    int nilai = deck->head->nilaiKartu;
    nodeKartu *temp = deck->head;
    while (temp != deck->tail)
    {
        temp = temp->next;
        if (temp->nilaiKartu != nilai)
        {
            return false;
        }
    }
    *highest = deck->head->nilaiKartu;
    return true;
}

bool is_it_straight(pointKartu *deck, int *highest)
{
    nodeKartu *temp = deck->head;
    while (temp != deck->tail->next)
    {
        if (temp->nilaiKartu + 1 != temp->next->nilaiKartu)
        {
            return false;
        }
        temp = temp->next;
    }
    *highest = deck->tail->nilaiKartu;
    return true;
}

bool is_it_flush(pointKartu *deck, int *highest)
{
    nodeKartu *temp = deck->head;
    while (temp != deck->tail->next)
    {
        if (temp->tipeKartu != temp->next->tipeKartu)
        {
            return false;
        }
        temp = temp->next;
    }
    *highest = deck->tail->nilaiKartu;
    return true;
}

bool is_it_full_house(pointKartu *deck, int *highest)
{
    int frekuensi[16] = {0};

    nodeKartu *temp = deck->head;
    while (temp != deck->tail->next)
    {
        frekuensi[temp->nilaiKartu]++;
        temp = temp->next;
    }
    int hitungTiga = 0;
    int hitungDua = 0;

    for (int i = 3; i <= 15; i++)
    {
        if (frekuensi[i] == 3)
        {
            hitungTiga++;
            *highest = i;
        }
        else if (frekuensi[i] == 2)
        {
            hitungDua++;
        }
    }
    if (hitungTiga == 1 && hitungDua == 1)
    {
        return true;
    }
    return false;
}

bool is_it_four_of_a_kind(pointKartu *deck, int *highest)
{
    int nilai = deck->head->nilaiKartu;
    nodeKartu *temp = deck->head;
    while (temp != deck->tail->next)
    {
        if (temp->nilaiKartu != nilai)
        {
            return false;
        }
        temp = temp->next;
    }
    *highest = deck->tail->nilaiKartu;
    return true;
}

bool is_it_straight_flush(pointKartu *deck, int *highest)
{
    nodeKartu *head = deck->head;
    if (is_it_flush(deck, highest) && is_it_straight(deck, highest))
    {
        *highest = deck->tail->nilaiKartu;
        return true;
    }
    return false;
}

bool is_it_royal_flush(pointKartu *deck, int *highest)
{
    if (is_it_flush(deck, highest) && is_it_straight(deck, highest))
    {
        if (deck->tail->nilaiKartu == As || deck->tail->nilaiKartu == King)
        {
            *highest = deck->tail->nilaiKartu;
            return true;
        }
    }
    return false;
}

int compare_Meja(nodeMeja *asli, nodeMeja *sementara)
{
    int cekBreak = 0;
    // Jika cekBreak = 2, maka game akan selesai (bom)
    // Jika cekBreak = 1, maka akan terjadi perpindahan dari dek meja sementara menjadi dek meja asli
    // Jika cekBreak = 0, maka tidak akan terjadi perpindahan
    if (asli->aturan == 0)
    {
        asli->aturan = sementara->aturan;
        asli->nilaiTertinggi = sementara->nilaiTertinggi;
        asli->llDeck = sementara->llDeck;
        cekBreak = 1;
        return cekBreak;
    }

    if (asli->nilaiTertinggi == Poker && sementara->aturan == fourOfaKind && asli->aturan == highCard) // pengecekan bom
    {
        asli->aturan = sementara->aturan;
        asli->nilaiTertinggi = sementara->nilaiTertinggi;
        asli->llDeck = sementara->llDeck;
        cekBreak = 2;
        return cekBreak;
    }

    if (asli->nilaiTertinggi < sementara->nilaiTertinggi && asli->aturan == sementara->aturan)
    {

        asli->nilaiTertinggi = sementara->nilaiTertinggi;
        asli->llDeck = sementara->llDeck;
        cekBreak = 1;
        return cekBreak;
    }

    if (asli->nilaiTertinggi > sementara->nilaiTertinggi && asli->aturan == sementara->aturan)
    {
        cekBreak = 0;
        return cekBreak;
    }
    return cekBreak;
}

bool computer_turn(nodeMeja *dekMeja, nodeMeja *dekTemp, nodePemain *com)
{
    pointKartu *llComb = (pointKartu *)malloc(sizeof(pointKartu));
    llComb->head = com->kartu.head;
    llComb->tail = NULL;
    bool result = false;

    if (dekMeja->aturan == 0)
    {
        do
        {
            int choice = (rand() % 5) + 1;
            switch (choice)
            {
            case 1:
            {
                puts("TESSS SATU");
                nodeKartu *tempHead = com->kartu.head;
                llComb->head = tempHead;
                llComb->tail = tempHead;
                tempHead = tempHead->next;
                com->kartu.head = tempHead;
                llComb->tail->next = NULL;
                dekMeja->llDeck = llComb;
                result = true;
                puts("TESSS SATU");
                break;
            }
            case 2:
            {
                puts("TESSS DUA");
                result = two_cards_comb(llComb, dekMeja, dekTemp);
                puts("TESSS DUA");
                break;
            }
            case 3:
            {
                puts("TESSS TIGA");
                result = three_cards_comb(llComb, dekMeja, dekTemp);
                puts("TESSS TIGA");
                break;
            }
            case 5:
            {
                puts("TESSS LIMA");
                result = five_cards_comb(llComb, dekMeja, dekTemp);
                puts("TESSS LIMA");
                break;
            }
            default:
            {
                result = false;
                break;
            }
            }

        } while (!result);
    }
    else
    {
        switch (dekMeja->aturan)
        {
        case highCard:
            puts("-----------------1");
            if (dekMeja->nilaiTertinggi == Poker)
            {
                result = four_cards_comb(llComb, dekMeja, dekTemp);
                if (!result)
                {
                    result = high_card_fight(llComb, dekMeja, dekTemp);
                }
            }
            puts("-----------------1");
            break;
        case pair:
            puts("-----------------2");
            result = two_cards_comb(llComb, dekMeja, dekTemp);
            puts("-----------------2");
            break;
        case threeOfaKind:
            puts("-----------------3");
            result = three_cards_comb(llComb, dekMeja, dekTemp);
            puts("-----------------3");
            break;
        default:
            puts("-----------------5");
            if (four_cards_comb(llComb, dekMeja, dekTemp))
            {
                result = true;
            }
            else
            {
                result = five_cards_comb(llComb, dekMeja, dekTemp);
            }
            puts("-----------------5");
            break;
        }
    }
    if (result)
    {
        free_memory_deck(dekMeja);
        get_ll_comb(com, llComb);
        dekMeja->aturan = dekTemp->aturan;
        dekMeja->nilaiTertinggi = dekTemp->nilaiTertinggi;
        dekMeja->llDeck = llComb;
    }
    free(llComb);
    return result;
}

void set_tail(pointKartu *llComb, int card)
{
    int jumlah = 1;
    nodeKartu *temp = llComb->head;
    while (jumlah < card)
    {
        temp = temp->next;
        jumlah++;
    }
    llComb->tail = temp;
}

bool five_cards_comb(pointKartu *llComb, nodeMeja *dekMeja, nodeMeja *dekTemp)
{
    nodeKartu *temp = llComb->head;
    bool result = false;

    if (dekMeja->aturan <= royalFlush && dekMeja->aturan >= straight)
    {
        set_tail(llComb, 5);
        while (llComb->tail != NULL)
        {
            if (is_it_straight_flush(llComb, &(dekMeja->nilaiTertinggi)) && dekMeja->aturan <= straightFlush)
            {
                if (dekTemp->llDeck->tail->tipeKartu == dekMeja->llDeck->tail->tipeKartu)
                {
                    if (dekTemp->llDeck->tail->nilaiKartu > dekMeja->llDeck->tail->nilaiKartu)
                    {
                        dekTemp->aturan = straightFlush;
                        result = true;
                        break;
                    }
                    else
                    {
                        result = false;
                    }
                }
                else if (dekTemp->llDeck->tail->tipeKartu > dekMeja->llDeck->tail->tipeKartu)
                {
                    dekTemp->aturan = straightFlush;
                    result = true;
                    break;
                }
                else
                {
                    result = false;
                }
            }
            else if (is_it_royal_flush(llComb, &(dekMeja->nilaiTertinggi)) && dekMeja->aturan <= royalFlush)
            {
                if (dekTemp->llDeck->tail->tipeKartu > dekMeja->llDeck->tail->tipeKartu)
                {
                    dekTemp->aturan = royalFlush;
                    result = true;
                    break;
                }
                else
                {
                    result = false;
                }
            }
            llComb->head = llComb->head->next;
            llComb->tail = llComb->tail->next;
        }
    }

    if (!result)
    {
        llComb->head = temp;
        set_tail(llComb, 5);
        while (llComb->tail != NULL)
        {
            if (dekMeja->aturan <= straight)
            {
                if (is_it_straight(llComb, &(dekTemp->nilaiTertinggi)))
                {
                    if (dekMeja->aturan == 0)
                    {
                        dekTemp->aturan = straight;
                        result = true;
                        break;
                    }
                    else
                    {
                        if (dekTemp->nilaiTertinggi == dekMeja->nilaiTertinggi)
                        {
                            if (dekTemp->llDeck->tail->tipeKartu > dekMeja->llDeck->tail->tipeKartu)
                            {
                                dekTemp->aturan = straight;
                                result = true;
                                break;
                            }
                            else
                            {
                                result = false;
                            }
                        }
                        else if (dekTemp->nilaiTertinggi > dekMeja->nilaiTertinggi)
                        {
                            dekTemp->aturan = straight;
                            result = true;
                            break;
                        }
                        else
                        {
                            result = false;
                        }
                    }
                }
            }
            if (dekMeja->aturan <= flush)
            {
                if (is_it_flush(llComb, &(dekTemp->nilaiTertinggi)))
                {
                    if (dekMeja->aturan == 0)
                    {
                        dekTemp->aturan = flush;
                        result = true;
                        break;
                    }
                    else
                    {
                        if (dekTemp->llDeck->tail->tipeKartu == dekMeja->llDeck->tail->tipeKartu)
                        {
                            if (dekTemp->llDeck->tail->nilaiKartu > dekMeja->llDeck->tail->nilaiKartu)
                            {
                                dekTemp->aturan = flush;
                                result = true;
                                break;
                            }

                            else
                            {
                                result = false;
                            }
                        }
                        else if (dekTemp->llDeck->tail->tipeKartu > dekMeja->llDeck->tail->tipeKartu)
                        {
                            dekTemp->aturan = flush;
                            result = true;
                            break;
                        }
                        else
                        {
                            result = false;
                        }
                    }
                }
            }
            if (dekMeja->aturan <= fullHouse)
            {
                if (is_it_full_house(llComb, &(dekTemp->nilaiTertinggi)))
                {
                    if (dekMeja->aturan == 0)
                    {
                        dekTemp->aturan = fullHouse;
                        result = true;
                        break;
                    }
                    else
                    {
                        if (dekTemp->nilaiTertinggi > dekMeja->nilaiTertinggi)
                        {
                            dekTemp->aturan = fullHouse;
                            result = true;
                            break;
                        }
                        else
                        {
                            result = false;
                        }
                    }
                }
            }
            llComb->head = llComb->head->next;
            llComb->tail = llComb->tail->next;
        }
    }
    if (!result)
        llComb->head = temp;
    return result;
}

bool four_cards_comb(pointKartu *llComb, nodeMeja *dekMeja, nodeMeja *dekTemp)
{
    nodeKartu *temp = llComb->head;
    set_tail(llComb, 4);
    while (llComb->tail != NULL)
    {
        if (is_it_four_of_a_kind(llComb, &(dekTemp->nilaiTertinggi)))
        {
            dekTemp->aturan = fourOfaKind;
            return true;
        }
        llComb->head = llComb->head->next;
        llComb->tail = llComb->tail->next;
    }
    return false;
}

bool three_cards_comb(pointKartu *llComb, nodeMeja *dekMeja, nodeMeja *dekTemp)
{
    bool result = false;
    nodeKartu *temp = llComb->head;
    set_tail(llComb, 3);
    while (llComb->tail != NULL)
    {
        if (is_it_three_of_a_kind(llComb, &(dekTemp->nilaiTertinggi)))
        {
            if (dekMeja->aturan == 0)
            {
                dekTemp->aturan = threeOfaKind;
                result = true;
                break;
            }
            else
            {
                if (dekTemp->nilaiTertinggi > dekMeja->nilaiTertinggi)
                {
                    dekTemp->aturan = threeOfaKind;
                    result = true;
                    break;
                }
                else
                {
                    result = false;
                }
            }
        }
        llComb->head = llComb->head->next;
        llComb->tail = llComb->tail->next;
    }
    if (!result)
        llComb->head = temp;
    return result;
}

bool two_cards_comb(pointKartu *llComb, nodeMeja *dekMeja, nodeMeja *dekTemp)
{
    bool result = false;
    nodeKartu *temp = llComb->head;
    set_tail(llComb, 2);
    while (llComb->tail != NULL && !result)
    {
        if (is_it_pair(llComb, &(dekTemp->nilaiTertinggi)))
        {
            if (dekMeja->aturan == 0)
            {
                dekTemp->aturan = pair;
                result = true;
                break;
            }
            else
            {
                if (dekTemp->nilaiTertinggi == dekMeja->nilaiTertinggi)
                {
                    if (dekTemp->llDeck->tail->tipeKartu > dekMeja->llDeck->tail->tipeKartu)
                    {
                        dekTemp->aturan = straight;
                        result = true;
                        break;
                    }
                    else
                    {
                        result = false;
                    }
                }
                else if (dekTemp->nilaiTertinggi > dekMeja->nilaiTertinggi)
                {
                    dekTemp->aturan = straight;
                    result = true;
                    break;
                }
                else
                {
                    result = false;
                }
            }
        }
        llComb->head = llComb->head->next;
        llComb->tail = llComb->tail->next;
    }
    if (!result)
        llComb->head = temp;

    return result;
}

bool high_card_fight(pointKartu *llComb, nodeMeja *dekMeja, nodeMeja *dekTemp)
{
    bool result = false;
    nodeKartu *temp = llComb->head;
    set_tail(llComb, 1);
    while (llComb->tail != NULL && !result)
    {
        if (is_it_high_card(llComb, &(dekTemp->nilaiTertinggi)))
        {
            if (dekMeja->aturan == 0)
            {
                result = true;
            }
            else
            {
                if (dekTemp->nilaiTertinggi == dekMeja->nilaiTertinggi)
                {
                    if (dekTemp->llDeck->tail->tipeKartu > dekMeja->llDeck->tail->tipeKartu)
                    {
                        dekTemp->aturan = straight;
                        result = true;
                        break;
                    }
                    else
                    {
                        result = false;
                    }
                }
                else if (dekTemp->nilaiTertinggi > dekMeja->nilaiTertinggi)
                {
                    dekTemp->aturan = straight;
                    result = true;
                    break;
                }
                else
                {
                    result = false;
                }
            }
        }
        llComb->head = llComb->head->next;
        llComb->tail = llComb->tail->next;
    }
    if (!result)
        llComb->head = temp;

    return result;
}

void free_memory_deck(nodeMeja *deck)
{
    if (deck->llDeck != NULL)
    {
        nodeKartu *temp = deck->llDeck->head;
        nodeKartu *next;
        while (temp)
        {
            next = temp->next;
            free(temp);
            temp = next;
        }
    }

    deck->aturan = 0;
    deck->nilaiTertinggi = 0;
    deck->llDeck = NULL;
}

void get_ll_comb(nodePemain *com, pointKartu *llComb)
{
    nodeKartu *temp = com->kartu.head;
    if (temp == llComb->head)
    {
        if (llComb->tail != NULL)
        {
            com->kartu.head = llComb->tail->next;
        }
        else
        {
            com->kartu.head = NULL;
        }
    }
    else
    {
        while (temp->next != llComb->head)
        {
            temp = temp->next;
        }
        temp->next = llComb->tail->next;
    }
    llComb->tail->next = NULL;
}

bool player_turn(nodePemain *player, nodeMeja *dekTemp)
{
    char choice;
    bool validInput = false;
    do
    {
        int card;
        printf("Lawan kartu di meja? (y/n): ");
        scanf(" %c", &choice);
        do
        {
            switch (choice)
            {
            case 'y':
                validInput = true;
                printf("pilih nomor kartu : ");
                scanf(" %d", &card);
                get_player_card(&(player)->kartu, dekTemp, card);
                if (dekTemp->llDeck == NULL)
                {
                    printf("KARTU DENGAN NOMOR %D TIDAK DITEMUKAN! PILIH KARTU LAIN\n\n", card);
                }
                else
                {
                    printf("Taruh kartu? (y/n): ");
                    scanf(" %c", &choice);
                }
                break;

            case 'n':
                return false;
                break;

            default:
                puts("input salah");
                break;
            }
        } while (choice != 'n');
    } while (!validInput);
}

void get_player_card(pointKartu *llPlayer, nodeMeja *dekTemp, int card)
{
    if (dekTemp->llDeck != NULL)
    {
        free_memory_deck(dekTemp);
        dekTemp->llDeck->head = NULL;
        dekTemp->llDeck->tail = NULL;
    }
    nodeKartu *selected = llPlayer->head;
    while (selected->nomorKartu != card)
    {
        selected = selected->next;
    }
    if (selected == NULL)
    {
        return;
    }
    if (selected == llPlayer->head)
    {
        llPlayer->head = selected->next;
    }
    else
    {
        nodeKartu *prev = llPlayer->head;
        while (prev->next != selected)
        {
            prev = prev->next;
        }
        prev->next = selected->next;
    }
    printf("%d\n", dekTemp->llDeck->head->nilaiKartu);
    insert_order(dekTemp->llDeck, selected);
    puts("TESSSSS PLAYER CARD 1");
}

void alloc_deck(nodeMeja **deck)
{
    *deck = (nodeMeja *)malloc(sizeof(nodeMeja));
    (*deck)->llDeck = NULL;
    (*deck)->aturan = 0;
    (*deck)->nilaiTertinggi = 0;
}

int main()
{
    int cekBreak, choice, round;
    do
    {
        printf("\n\n\n\t\t\tSELAMAT DATANG DI POKER TONGKRONGAN \n");
        printf("MENU :\n\n");
        printf("1. Start\n\n2. Help Menu\n\n3. History\n\n4. Exit\n\n\nPilih Menu: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            char playerName[10];
            pointKartu dekLL = {NULL, NULL};
            nodePemain *aktif = NULL;
            nodePemain *com = NULL;
            pointKartu *dekAktif = NULL;
            nodePemain *player = NULL;
            nodeMeja *dekMeja = NULL;
            nodeMeja *dekTemp = NULL;
            /* start */
            system("cls");
            printf("\t\t\tMasukkan Nama Pemain (MAX 9 Huruf): "); // nama player
            scanf("%s", &playerName);
            system("cls");
            fill_the_card(&dekLL);
            shuffle_deck(&dekLL);
            create_player(&aktif, playerName);
            player = aktif;
            com = aktif->next;
            fill_deck(&aktif, &dekLL);
            aktif = first_play(aktif);
            alloc_deck(&dekMeja);
            alloc_deck(&dekTemp);
            nodePemain *winner = aktif;
            printf("PERTAMA MAIN : %s\n\n", aktif->nama);
            bool fight = false;
            do
            {
                print_game_computers(com, playerName);
                puts("\n\nDEK MEJA :");
                if (dekMeja->llDeck != NULL)
                {
                    print_game_card(dekMeja->llDeck);
                    print_rule_table(dekMeja);
                    printf("%s\n", winner->nama);
                    puts("\n");
                }
                else
                {
                    puts("KOSONG\n\n");
                }
                print_game_card(&(player)->kartu);
                if (aktif != player)
                {
                    if (aktif->kartu.head != NULL && aktif->kartu.head)
                    {
                        printf("\n%s sedang berpikir...\n", aktif->nama);
                        system("pause\n");
                        fight = computer_turn(dekMeja, dekTemp, aktif);
                        if (fight)
                        {
                            winner = aktif;
                        }
                        else
                        {
                            printf("\n%s skip\n", aktif->nama);
                        }
                    }
                    else
                    {
                        nodePemain *prev = aktif;
                        while (prev->next != aktif)
                        {
                            prev = prev->next;
                        }
                        prev->next = aktif->next;
                        nodePemain *temp = aktif;
                        aktif = prev;
                        free(temp);
                    }
                }
                else
                {
                    while (true)
                    {
                        fight = player_turn(player, dekTemp);
                        if (fight)
                        {
                            cekBreak = compare_Meja(dekMeja, dekTemp);
                            switch (cekBreak)
                            {
                            case 0:
                                return_card(dekTemp->llDeck, &(player)->kartu);
                                printf("KARTU YANG DIPILIH TIDAK BISA MELAWAN, PILIH KARTU LAIN? (y/n): ");
                                scanf(" %c", &choice);
                                if (choice == 'y')
                                {
                                    continue;
                                }
                                else if (choice == 'n')
                                {
                                    break;
                                }
                                break;
                            case 1:
                                free_memory_deck(dekMeja);
                                dekMeja = dekTemp;
                                winner = player;
                                break;
                            case 2:
                                winner = player;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                aktif = aktif->next;
                if (aktif == winner)
                {
                    free_memory_deck(dekMeja);
                }

                // system("cls");
            } while ((player->kartu.head && player->next != NULL));
            break;

            // case 2:
            //     // help
            //     break;
            // case 3:
            //     // history
            //     break;
            // default:
            //     break;
        }
    } while (choice != 4);
}

void return_card(pointKartu *llTemp, pointKartu *llPlayer)
{
    nodeKartu *temp2 = llTemp->head;
    nodeKartu *nextNode;

    while (temp2 != NULL)
    {
        nextNode = temp2->next;
        insert_order(llPlayer, temp2);
        temp2 = nextNode;
    }

    llTemp->head = NULL;
    llTemp->tail = NULL;
}

void print_rule_table(nodeMeja *dekMeja)
{
    int cek = dekMeja->aturan;
    switch (cek)
    {
    case 0:
        puts("DEK MEJA MASIH KOSONG");
        break;
    case 1:
        puts("HIGH CARD");
        break;
    case 2:
        puts("PAIR");
        break;
    case 3:
        puts("THREE OF A KIND");
        break;
    case 4:
        puts("STRAIGHT");
        break;
    case 5:
        puts("FLUSH");
        break;
    case 6:
        puts("FULL HOUSE");
        break;
    case 7:
        puts("FOUR OF A KIND");
        break;
    case 8:
        puts("STRAIGHT FLUSH");
        break;
    case 9:
        puts("ROYAL FLUSH");
        break;
    }
}
