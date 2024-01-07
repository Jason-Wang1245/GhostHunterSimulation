#include "defs.h"

int main()
{
    // // Initialize the random number generator
    // srand(time(NULL));

    // // Create the house: You may change this, but it's here for demonstration purposes
    // // Note: This code will not compile until you have implemented the house functions and structures

    House house;
    pthread_t gThread;
    pthread_t hThreads[NUM_HUNTERS];
    initHouse(&house);
    populateRooms(&house);
    populateGhost(&house);
    populateHunters(&house);

    pthread_create(&gThread, NULL, ghostThread, house.ghost);
    HunterNode *curNode = house.hunters.head;
    for (int i = 0; i < NUM_HUNTERS; i++){
        pthread_create(hThreads+i, NULL, hunterThread, curNode->data);
        curNode = curNode->next;
    }

    
    pthread_join(gThread, NULL);
    for (int i = 0; i < NUM_HUNTERS; i++){
        pthread_join(hThreads[i], NULL);
    }

    printResults(&house.hunters, house.ghost, house.sharedEvidence);

    cleanUpHouse(&house);

    return 0;


}

void printResults(HunterList *hunters, Ghost *ghost, EvidenceList *evidenceList){
    int counter = 0;
    char str[MAX_STR];

    printf("=============\n");
    printf("Final Results\n");
    printf("=============\n");
    HunterNode *curHuNode = hunters->head;
    while (curHuNode != NULL){
        if (curHuNode->data->exitReason == EXIT_FEAR){
            printf(" * Hunter %s has run away in fear\n", curHuNode->data->name);
            counter++;
        }
        if (curHuNode->data->exitReason == EXIT_BORED){
            printf(" * Hunter %s has gotten too bored to continue\n", curHuNode->data->name);
            counter++;
        }
        curHuNode = curHuNode->next;
    }

    if (counter == NUM_HUNTERS){
        printf("The ghost has won.\n");
    } else {
        printf("The Hunters won.\n");
    }

    ghostToString(ghost->classType, str);
    if (counter == NUM_HUNTERS){    
        printf("The ghost is actually a %s\n", str);
    } else {
        printf("The hunters have correctly guess that the ghost is a %s\n", str);
    }

    printf("Here is the list of evidence that the hunters have collected: \n");
    EvidenceNode *curEvNode = evidenceList->head;
    while (curEvNode != NULL){
        evidenceToString(curEvNode->data->evidenceType, str);
        printf(" * %s\n", str);
        curEvNode = curEvNode->next;
    }

    

}
