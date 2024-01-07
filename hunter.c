#include "defs.h"

// input: EvidenceType equipment, EvidenceList *evidenceCollection, char *name, Hunter **hunter
// output: void
// input/output: Hunter **hunter
// purpose: Intializes values of hunter with provided parameters and dynamically allocating space for ghost and setting the pointer to it
void initHunter(EvidenceType equipment, Room *room, EvidenceList *evidenceCollection, char name[], Hunter **hunter){
    Hunter *hunterPointer = (Hunter *) calloc(sizeof(Hunter), 1);
    if (hunterPointer == NULL){
        printf("Memory Allocation Error.\n");
        return;
    }
    hunterPointer->equipment = equipment;
    hunterPointer->room = room;
    hunterPointer->evidenceCollection = evidenceCollection;
    strcpy(hunterPointer->name, name);
    hunterPointer->fear = 0;
    hunterPointer->boredom = 0;
    hunterPointer->exitReason = EXIT_UNKNOWN;
    *hunter = hunterPointer;
}

// input: Hunter *hunter
// output: int (boolean value)
// output: Hunter *hunter
// purpose: Increments the fear counter of hunter by 1, if it greater than or equal to FEAR_MAX, return C_TRUE, return C_FALSE otherwise
int incrementHunterFear(Hunter *hunter){
    hunter->fear = hunter->fear + 1;
    if (hunter->fear >= FEAR_MAX) return C_TRUE;
    else return C_FALSE;
}

// input: Hunter *hunter
// output: void
// output: Hunter *hunter
// purpose: Increments the boredom counter of hunter by 1
void incrementHunterBoredom(Hunter *hunter){
    hunter->boredom = hunter->boredom + 1;
}

// input: Hunter *hunter
// output: int (boolean value)
// input/output: n/a
// purpuse: Checks if a hunters boredom value has reached BOREDOM_MAX, if so return C_TRUE, return C_FALSE otherwise
int hunterBored(Hunter *hunter){
    return hunter->boredom >= BOREDOM_MAX;
}

// input: Hunter *hunter
// output: void
// output: Hunter *hunter
// purpose: Sets the boredom value of hunter to 0
void resetHunterBoredom(Hunter *hunter){
    hunter->boredom = 0;
}

// input: Room *room, Hunter *hunter
// output: void
// output: Hunter *hunter
// purpose: Changes the room that the hunter is currently in to the input room
void moveRoomHunter(Room *room, Hunter *hunter){
    hunter->room = room;
}


// input: HunterList *hunterList
// output: void
// input/output: HunterList *hunterList
// purpose: Initializes the hunterList with NULL values for the head and tail nodes
void initHunterList(HunterList *hunterList){
    hunterList->head = NULL;
    hunterList->tail = NULL;
}

// input: Hunter *hunter, HunterList *hunterList
// output: void
// input/output: HunterList *hunterList
// purpose: Adds hunter to the end of hunterList
void addHunter(Hunter *hunter, HunterList *hunterList){
    HunterNode *hunterNode = (HunterNode *) calloc(1, sizeof(HunterNode));
    if (hunterNode == NULL){
        printf("Memory Allocation Error.\n");
        return;
    }
    hunterNode->data = hunter;
    hunterNode->next = NULL;
    // edge case: hunterList is empty
    if (hunterList->head == NULL){
        hunterList->head = hunterNode;
        hunterList->tail = hunterNode;
    } else {
        hunterList->tail->next = hunterNode;
        hunterList->tail = hunterNode;
    }
}

// input: Hunter *hunter, HunterList *hunterList
// output: int (boolean value)
// input/output: HunterList *hunterList
// purpose: If hunter is in hunterList, remove it and return C_TRUE if the element was removed, C_FALSE otherwise
int removeHunter(Hunter *hunter, HunterList *hunterList){
    if (hunterList->head == NULL) return C_FALSE; // edge case: there are no items in hunterList
    // edge case: only one element in hunterList
    if (hunterList->head == hunterList->tail){
        if (hunterList->head->data == hunter){
            free(hunterList->head);
            hunterList->head = NULL;
            hunterList->tail = NULL;
            return C_TRUE;
        } else return C_FALSE;
    }

    HunterNode *prevNode = hunterList->head;
    HunterNode *curNode = prevNode->next;
    // edge case: The head node is the node that needs to be removed
    if (prevNode->data == hunter){
        free(prevNode);
        hunterList->head = curNode;
        return C_TRUE;
    }

    while (curNode != NULL){
        if (curNode->data == hunter){
            // edge case: The tail node is the node that needs to be removed
            if (curNode->next == NULL){
                hunterList->tail = prevNode;
                prevNode->next = NULL;
            } else {
                prevNode->next = curNode->next;
            }
            free(curNode);
            return C_TRUE;
        }
        prevNode = curNode;
        curNode = curNode->next;
    }
    // no element was removed at this point
    return C_FALSE; 
}

// input: HunterList *hunterList
// output: void
// input/output: HunterList *hunterList
// purpose: Frees dynamically located memory for all data in hunterList
void cleanUpHunterData(HunterList *hunterList){
    HunterNode *curNode = hunterList->head;
    while (curNode != NULL){
        free(curNode->data);
        curNode = curNode->next;
    }
}

// input: HunterList *hunterList
// output: void
// input/output: HunterList *hunterList
// purpose: Frees dynamically located memory for all nodes in hunterList
void cleanUpHunterNodes(HunterList *hunterList){
    HunterNode *curNode = hunterList->head;
    HunterNode *prevNode;
    while(curNode != NULL){
        prevNode = curNode;
        curNode = curNode->next;
        free(prevNode);
    }
}