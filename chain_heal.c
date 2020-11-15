
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct node
{
    int visited;
    int adj_size;
    struct node **adj;
    char *name;
    int x, y;
    int cur_PP, max_PP;
    struct node *next;
} Node;

typedef struct data {
    int initial_range, jump_range, num_jumps, initial_power;
    double power_reduction;
} Data;

int best_path_length;
Node **best_path;
int totalHealing;
int bestHealing;
Node **temp_path;
int *healing;
void add (Node *input, char *tempNameInitial, int tempx, int tempy, int tempCur_PP, int tempMax_PP) {
    int length = strlen(tempNameInitial);
    input->name = (char *) malloc(sizeof(char)*length);
    int i;

    for (i = 0; tempNameInitial[i] != '\0'; i++) { // Cuts down on Bytes?
        input->name[i] = tempNameInitial[i];
    }

    input->x = tempx;
    input->y = tempy;
    input->cur_PP = tempCur_PP;
    input->max_PP = tempMax_PP;

    input->next = malloc(sizeof(Node));
}

// Need to loop this- creates graph for one
void createGraph (Node *input, int size) {
    input->adj = malloc(sizeof(Node *) * size);
    input->adj_size = size;

/*
    int c;
    for (c = 0; c < size; c++) {
        input->adj[c] = NULL;
    }
*/
}


void edge (Node *head, Node *comparison, Data data) {
    int c;
    Node *current = head;

    int count = 0;

//while (count < comparison->adj_size) {
    while (current->next != NULL) {
        double distance = sqrt(pow(comparison->x-current->x, 2) + pow(comparison->y-current->y, 2));
        if ((current->x != comparison->x) || (current->y != comparison->y)){
            if (distance <= data.jump_range) {
                comparison->adj[count] = current;
                int a;
                printf("This is what's going in the adj list");
                for (a = 0; current->name[a] != '\0'; a++) {
                    printf("%c", current->name[a]);
                }

                count++;
                printf("Current X: %d Comparison X: %d Current Y: %d Comparison Y: %d \n", current->x, comparison->x, current->y, comparison->y);
            }
        }
        current = current->next;
    }
    printf("Count: %d ", count);
}


// Need to put in createGraph method call- creates size of adjs for one
int numEdges (Node *head, Node *comparison, Data data) {
    int size = 0;
    Node *current = head;
    while (current->next != NULL) {
        double distance = sqrt(pow(comparison->x-current->x, 2) + pow(comparison->y-current->y, 2));
        printf("Distance %f \n", distance);
        if (distance <= data.jump_range) {

            printf("Distance within jumping Range %f \n", distance);
            size++;
        }
        current = current->next;
    }

    return size-1;
}




void DFS (Node *head, int hop, Data data) {

    Node *current = head;
    while (current->next != NULL) {
        double distance = sqrt(pow(head->x-current->x, 2) + pow(head->y-current->y, 2));
        if (hop == 1) {
            if (distance <= data.initial_range) {
                current->visited = 1;
                printf("\nName: ");
                int c;
                for (c = 0; current->name[c] != '\0' ; c++) {
                    printf("%c", current->name[c]);
                }
                printf(" Hop Number: %d\n", hop);

                int availableHealing = current->max_PP - current->cur_PP;

                if (availableHealing > data.initial_power) {
                    totalHealing = data.initial_power;
                    if (totalHealing > bestHealing) {
                        bestHealing = totalHealing;
                        best_path_length = 1;
                    }
                }
                else {
                    totalHealing = availableHealing;
                    if (totalHealing > bestHealing) {
                        bestHealing = totalHealing;
                        best_path_length = 1;
                    }
                }


                printf("AVAILABLE HEALING: %d ", availableHealing);
                printf("TOTALHEALING: %d\n", totalHealing);
                printf("BESTHEALING: %d ", availableHealing);


                temp_path[0] = current;
                healing[0] = totalHealing;
                DFS (current, hop+1, data);

                current->visited = 0;
            }
        }
        current = current->next;
    }

    if (hop != 1 && hop <= data.num_jumps) {
        int c = 0;
        while (c < head->adj_size) {
            if (head->adj[c]->visited == 0) {
                current = head->adj[c];
                current->visited = 1;

                printf("\nName: ");
                int i;
                for (i = 0; current->name[i] != '\0'; i++) {
                    printf("%c", current->name[i]);
                }
                printf(" Hop Number: %d\n", hop);

                int availableHealing = current->max_PP - current->cur_PP;
                int healingFactor = (data.initial_power*pow(1-data.power_reduction,hop-1));

                if (availableHealing > healingFactor) {
                    totalHealing += healingFactor;
                    printf("AVAILABLE HEALING: %d\n", availableHealing);
                    printf("HEALINGFACTOR: %d\n", healingFactor);
                    printf("TOTALHEALING: %d\n", totalHealing);


                    temp_path[hop-1] = current;
                    healing[hop-1] = healingFactor;

                    if (totalHealing > bestHealing) {
                        printf("Best Healing: %d\n", bestHealing);
                        printf("Total Healing: $%d\n\n", totalHealing);

                        //printf("SOMETHING HAPPENED111111");
                        bestHealing = totalHealing;
                        int b;
                        for (b = 0; b < data.num_jumps; b++) {
                            best_path[b] = temp_path[b];
                            best_path_length = hop;
                        }
                    }


                    DFS(current, hop+1, data);
                    totalHealing-= healingFactor;

                }
                else {
                    totalHealing += availableHealing;
                    printf("AVAILABLE HEALING: %d\n", availableHealing);
                    printf("HEALINGFACTOR: %d\n", healingFactor);
                    printf("TOTALHEALING: %d\n", totalHealing);


                    temp_path[hop-1] = current;
                    healing[hop-1] = availableHealing;

                    if (totalHealing > bestHealing) {
                        printf("Best Healing: %d\n", bestHealing);
                        printf("Total Healing: $%d\n\n", totalHealing);
                        //printf("SOMETHING HAPPENED222222");
                        bestHealing = totalHealing;
                        int b;
                        for (b = 0; b < data.num_jumps; b++) {
                            best_path[b] = temp_path[b];
                            best_path_length = hop;
                        }
                    }
                    DFS(current, hop+1, data);
                    totalHealing -= availableHealing;
                }
            }
            current->visited = 0;
            c++;
        }
    }
}


void printAllAdj(Node *head) {
    Node *current = head;
    while (current->next != NULL) {

        int i = 0;
        int c = 0;
        Node *check = current->adj[i];
        int f = 0;

        printf("\nName: ");
        for (c = 0; current->name[c] != '\0'; c++) {
            printf("%c", current->name[c]);
        }
        c = 0;

        printf("\n");
        while (f < current->adj_size) {
            for (c = 0; check->name[c] != '\0'; c++) {
                printf("%c", check->name[c]);
            }
            printf("\n");
            i++;
            f++;
            check=current->adj[i];
            printf("This is C %d\n", i);
        }
        current = current->next;
    }
}


int main(int argc, char **argv) {

    int c;
    Node *head;
    Data data;

    data.initial_range = atoi(argv[1]);
    data.jump_range = atoi(argv[2]);
    data.num_jumps = atoi(argv[3]);
    data.initial_power = atoi(argv[4]);
    sscanf(argv[5], "%lf", &data.power_reduction);

    char name[101];
    int x;
    int y;
    int cur_PP;
    int max_PP;

    scanf("%d %d %d %d %s", &x,
          &y, &cur_PP, &max_PP, name);

    head = (Node *) malloc(sizeof(Node));
    add(head, name, x, y, cur_PP, max_PP);

    Node *current = head->next;
// scans in the rest of the nodes after head
    while (scanf("%d %d %d %d %s", &x,
                 &y, &cur_PP, &max_PP, name) == 5) {
        add(current, name, x, y, cur_PP, max_PP);

        current = current->next;
    }
    current = head;
    // counting how many nodes are there

    //prints out the text file
    while (current->next != NULL) {

        printf("Works %d %d %d %d ", current->x, current->y, current->cur_PP, current->max_PP);

        for (c = 0; current->name[c] != '\0'; c++) {
            printf("%c", current->name[c]);
        }
        printf("\n");
        current = current->next;
    }
    current = head;

    int test = 0;
    // Declares the graphs for the nodes
    while (current->next != NULL) {

        for (c = 0; current->name[c] != '\0'; c++) {
            printf("%c", current->name[c]);
        }
        printf("\n");

        createGraph(current, numEdges(head, current, data));
        edge(head, current, data);
        printf("Number of edges: %d \n \n", current->adj_size);
        current = current->next;
    }


    printf("\nTESTING \n");
    current = head;
    // Print to check
    while (current->next != NULL) {

        printf("size: %d ", current->adj_size);

        for (c = 0; current->name[c] != '\0'; c++) {
            printf("%c", current->name[c]);
        }

        printf("\n");

        current = current->next;
    }

    printf("Argc value: %d \n", argc);

    if (argc != 6) {
        fprintf(stderr, "usage:argument count to correct number\n"); exit(1);
    }

    printf("PrintAll \n");
    printAllAdj(head);
    printf("\nThis is THE DFS");

    temp_path = malloc(sizeof(Node *) * data.num_jumps);
    best_path = malloc(sizeof(Node *) * data.num_jumps);
    healing = malloc(sizeof(int)*data.num_jumps);
    DFS (head, 1, data);
    printf("\nBest Length: %d\n", best_path_length);
    printf("Best Total Healing: %d\n", bestHealing);
    printf("Best Path:\n\n");
    c = 0;
    int f = 0;

    while (f < best_path_length) {
        for (c = 0; best_path[f]->name[c] != '\0'; c++) {
            printf("%c", best_path[f]->name[c]);
        }
        printf("\nHealing: %d\n\n", healing[f]);

        f++;
    }


    exit(0);
}
