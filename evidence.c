#include "defs.h"

// input: EvidenceType evidenceType, char *ghostName, Evidence **evidence
// output: void
// input/output: Evidence **evidence
// purpose: Dynamially allocates space for evidence, and set its attributes to other parameter inputs
void initEvidence(EvidenceType evidenceType, char *ghostName, Evidence **evidence){
    Evidence *evidencePointer = (Evidence *) calloc(1, sizeof(Evidence));
    if (evidencePointer == NULL) {
        printf("Memory Allocation Error.\n");
        return;
    }
    evidencePointer->ghostName = ghostName;
    evidencePointer->evidenceType = evidenceType;
    *evidence = evidencePointer;
}

// input: EvidenceList *evidenceList
// output: void
// input/output: EvidenceList *evidenceList
// purpose: Initializes head and tail of evidenceList to NULL and initializes the semaphore
void initEvidenceList(EvidenceList *evidenceList){
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
    sem_init(&(evidenceList->mutex), 0, 1);
}

// input: EvidenceType evidenceType, char *ghostName
// output: void
// input/output: EvidenceList * evidenceList
// purpose: Adds evidence to the end of evidenceList if evidence is not already in the list and return C_TRUE (same if parameters evidence and a element in the list has the same evidenceType and ghostName), return C_FALSE otherwise
int addEvidence(EvidenceType evidenceType, char *ghostName, EvidenceList * evidenceList){
    EvidenceNode *curNode = evidenceList->head;
    // checks if the same piece of evidence is already in the list
    while (curNode != NULL){
        if ((curNode->data->evidenceType == evidenceType) && (strcmp(curNode->data->ghostName, ghostName) == 0)){
            return C_FALSE;
        }
        curNode = curNode->next;
    }
    
    Evidence *evidence;
    initEvidence(evidenceType, ghostName, &evidence);
    EvidenceNode *evidenceNode = (EvidenceNode *) calloc(1, sizeof(EvidenceNode));
    if (evidenceNode == NULL){
        printf("Memory Allocation Error.\n");
        return C_FALSE;
    }
    evidenceNode->data = evidence;
    evidenceNode->next = NULL;
    // edge case: evidenceList is empty
    if (evidenceList->head == NULL){
        evidenceList->head = evidenceNode;
        evidenceList->tail = evidenceNode;
    } else {
        evidenceList->tail->next = evidenceNode;
        evidenceList->tail = evidenceNode;
    }
    return C_TRUE;
}

// input: EvidenceList *evidenceList
// output: int (boolean value)
// input/output: n/a
// purpose: Checks if the there is enough evidence in the list (enough if the size of the list is EV_COUNT - 1) and return C_TRUE, return C_FALSE oherwise
int reviewEvidence(EvidenceList *evidenceList){
    int counter = 0;
    EvidenceNode *curNode = evidenceList->head;
    while (curNode != NULL){
        counter++;
        curNode = curNode->next;
    }
    return counter >= EV_COUNT - 1;
}

// input: EvidenceList *evidenceList
// output: void
// input/output: EvidenceList *evidenceList
// purpose: Frees dynamically located memory for all data in evidenceList
void cleanUpEvidenceData(EvidenceList *evidenceList){
    EvidenceNode *curNode = evidenceList->head;
    while (curNode != NULL){
        free(curNode->data);
        curNode = curNode->next;
    }
}

// input: EvidenceList *evidenceList
// output: void
// input/output: EvidenceList *evidenceList
// purpose: Frees dynamically located memory for all nodes in evidenceList
void cleanUpEvidenceNodes(EvidenceList *evidenceList){
    EvidenceNode *curNode = evidenceList->head;
    EvidenceNode *prevNode;
    while(curNode != NULL){
        prevNode = curNode;
        curNode = curNode->next;
        free(prevNode);
    }
}