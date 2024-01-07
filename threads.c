#include "defs.h"

// input: void *data
// output: void
// input/output: void *data
// purpose: Function for ghost thread. The input should be a pointer to the ghost
void *ghostThread(void *data){
    Ghost *ghost = (Ghost *) data;
    Room *initRoom;
    Room *movedRoom;
    RoomNode *curNode;
    int choice;
    int pickedEvidence;
    int droppableEvidence[EV_COUNT - 1];
    
    ghostTypeToEvidenceType(ghost, droppableEvidence);

    while (!ghostBored(ghost)){
        usleep(GHOST_WAIT);
        initRoom = ghost->room;

        // picks random room to move to; if moving
        curNode = initRoom->connectedRooms.head;
        for (int i = 0; i < randInt(0, initRoom->connectedRooms.size); i++){
            curNode = curNode->next;
        }
        movedRoom = curNode->data;

        choice = randInt(0, 3);

        // makes sure the lock sempahore in order of smallest address of sempahore first
        if (&initRoom->mutex < &movedRoom->mutex){
            sem_wait(&(initRoom->mutex));
            sem_wait(&(movedRoom->mutex));
        } else {
            sem_wait(&(movedRoom->mutex));
            sem_wait(&(initRoom->mutex));
        }

        // adjust counters as depending on if a hunter is in the room
        if (initRoom->hunters.head == NULL){
            incrementGhostBoredom(ghost);
        } else {
            resetGhostBoredom(ghost);
        }

        // ghost drops evidence 
        if (choice == 0){
            sem_post(&(movedRoom->mutex));
            pickedEvidence = droppableEvidence[randInt(0, 3)];
            addEvidenceInRoom(ghost, pickedEvidence, initRoom);
            l_ghostEvidence(pickedEvidence, initRoom->name);
            sem_post(&(initRoom->mutex));
        // ghost moves rooms; if there is not hunters currently in the room
        } else if (choice == 1) {
            // checks if there is a hunter in the room
            if (initRoom->hunters.head == NULL){    
                // move rooms
                moveRoomGhost(movedRoom, ghost);
                setGhostInRoom(NULL, initRoom);
                setGhostInRoom(ghost, movedRoom);
                l_ghostMove(movedRoom->name);
                sem_post(&(initRoom->mutex));
                sem_post(&(movedRoom->mutex));
            // ghost cant move because there is a hunter, re-roll the choice
            } else {
                sem_post(&(initRoom->mutex));
                sem_post(&(movedRoom->mutex));
                continue;
            }
        // ghost does nothing
        } else {
            sem_post(&(initRoom->mutex));
            sem_post(&(movedRoom->mutex));
        }
    }
    sem_wait(&(ghost->room->mutex));
    setGhostInRoom(NULL, ghost->room);
    l_ghostExit(LOG_BORED);
    sem_post(&(ghost->room->mutex));
    ghost->room = NULL;
    ghost->exitReason = EXIT_BORED;
    return NULL;
}

// input: void *data
// outpout: void
// input/output: void *data
// purpose: Function for hunter threads. The input should be a pointer to the hunter
void *hunterThread(void *data){
    Hunter *hunter = (Hunter *) data;
    Room *initRoom;
    Room *movedRoom;
    RoomNode *curNode;
    EvidenceNode *curEvNode;
    Evidence *curEv;
    int evidenceType;
    int choice;
    
    while (!hunterBored(hunter)){
        usleep(HUNTER_WAIT);
        initRoom = hunter->room;
        // picks a random room to move to
        curNode = initRoom->connectedRooms.head;
        for (int i = 0; i < randInt(0, initRoom->connectedRooms.size); i++){
            curNode = curNode->next;
        }
        movedRoom = curNode->data;

        choice = randInt(0, 3);

        // makes sure the lock sempahore in order of smallest address of sempahore first
        if (&initRoom->mutex < &movedRoom->mutex){
            sem_wait(&(initRoom->mutex));
            sem_wait(&(movedRoom->mutex));
        } else {
            sem_wait(&(movedRoom->mutex));
            sem_wait(&(initRoom->mutex));
        }

        // adjusts counters based on if there is ghost in the room
        if (initRoom->ghost == NULL){
            incrementHunterBoredom(hunter);
        } else {
            // The hunter fear has reached FEAR_MAX and exits thread
            if (incrementHunterFear(hunter)){
                removeHunterInRoom(hunter, hunter->room);
                l_hunterExit(hunter->name, LOG_FEAR);
                sem_post(&(initRoom->mutex));
                sem_post(&(movedRoom->mutex));
                hunter->room = NULL;
                hunter->exitReason = EXIT_FEAR;
                pthread_exit(NULL);
            }
            resetHunterBoredom(hunter);
        }

        // hunter moves into a different room
        if (choice == 0){
            moveRoomHunter(movedRoom, hunter);
            removeHunterInRoom(hunter, initRoom);        
            addHunterInRoom(hunter, movedRoom);
            l_hunterMove(hunter->name, movedRoom->name);
        // hunter collects evidence
        } else if(choice == 1){
            curEvNode = initRoom->droppedEvidence.head;
            while (curEvNode != NULL){
                // the room has a piece of evidence that is equivalent to the hunters equipment and adds it to colection of evidence between hunters
                if (curEvNode->data->evidenceType == hunter->equipment){
                    evidenceType = curEvNode->data->evidenceType;
                    curEv = curEvNode->data;

                    sem_wait(&hunter->evidenceCollection->mutex);
                    addEvidence(evidenceType, curEv->ghostName, hunter->evidenceCollection);
                    l_hunterCollect(hunter->name, evidenceType, initRoom->name);
                    sem_post(&hunter->evidenceCollection->mutex);
                }
                curEvNode = curEvNode->next;
            }
        } else {
            sem_wait(&hunter->evidenceCollection->mutex);
            if (reviewEvidence(hunter->evidenceCollection)){
                l_hunterReview(hunter->name, LOG_SUFFICIENT);
                sem_post(&hunter->evidenceCollection->mutex);
                removeHunterInRoom(hunter, hunter->room);
                l_hunterExit(hunter->name, LOG_EVIDENCE);
                sem_post(&(initRoom->mutex));
                sem_post(&(movedRoom->mutex));
                hunter->room = NULL;
                hunter->exitReason = EXIT_EVIDENCE;
                pthread_exit(NULL);
            } else {
                l_hunterReview(hunter->name, LOG_INSUFFICIENT);
            }
            sem_post(&hunter->evidenceCollection->mutex);
        }

        sem_post(&(initRoom->mutex));
        sem_post(&(movedRoom->mutex));
    }
    sem_wait(&hunter->room->mutex);
    removeHunterInRoom(hunter, hunter->room);
    hunter->room = NULL;
    hunter->exitReason = EXIT_BORED;
    l_hunterExit(hunter->name, LOG_BORED);
    sem_post(&hunter->room->mutex);
    return NULL;
}