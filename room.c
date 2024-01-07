#include "defs.h"

// input: char *name
// output: Room *
// input/output: n/a
// purpose: Creates a dynamically allocated room with a pointer to and it returns that
Room * createRoom(char *name){
    Room *roomPointer = (Room *) calloc(1, sizeof(Room));
    if (roomPointer == NULL){
        printf("Memory Allocation Error.\n");
        return NULL;
    }
    roomPointer->name = name;
    initRoomList(&roomPointer->connectedRooms);
    initEvidenceList(&roomPointer->droppedEvidence);
    initHunterList(&roomPointer->hunters);
    roomPointer->ghost = NULL;
    sem_init(&(roomPointer->mutex), 0, 1);
    return roomPointer;
}

// input: RoomList *roomList
// output: void
// input/output: RoomList *roomList
// purpose: Initialzes the values of roomList
void initRoomList(RoomList *roomList){
    roomList->head = NULL;
    roomList->tail = NULL;
    roomList->size = 0;
}

// input: Room *room, RoomList *roomList
// output: void
// input/output: RoomList *roomList
// purpose: Adds room to the end of roomList
void addRoom(Room *room, RoomList *roomList){
    RoomNode *roomNode = (RoomNode *) calloc(1, sizeof(RoomNode));
    if (roomNode == NULL){
        printf("Memory Allocation Error.\n");
        return;
    }
    roomNode->data = room;
    roomNode->next = NULL;
    // edge case: roomList is empty
    if (roomList->head == NULL){
        roomList->head = roomNode;
        roomList->tail = roomNode;
    } else {
        roomList->tail->next = roomNode;
        roomList->tail = roomNode;
    }
    roomList->size++;
}

// input: Room *roomOne, Room *roomTwo
// output: void
// input/output: Room *roomOne, Room *roomTwo
// purpose: Adds the two rooms into each others conenctedRooms list
void connectRooms(Room *roomOne, Room *roomTwo){
    addRoom(roomTwo, &roomOne->connectedRooms);
    addRoom(roomOne, &roomTwo->connectedRooms);
}

// input: Ghost *ghost, Room *room
// output: void
// intput/output: Room *room
// purpose: Sets the ghost as the current ghost in room
void setGhostInRoom(Ghost *ghost, Room *room){
    room->ghost = ghost;
}

// input: Hunter *hunter, Room *room
// output: void
// input/output: Room *room
// purpose: Adds the hunter to the list of hunters in room
void addHunterInRoom(Hunter *hunter, Room *room){
    addHunter(hunter, &room->hunters);
}

// input: Hunter *hunter, Room *room
// output: void
// input/output: Room *room
// purpose: Removes the hunter to the list of hunters in room
void removeHunterInRoom(Hunter *hunter, Room *room){
    removeHunter(hunter, &room->hunters);
}

// input: Ghost *ghost, EvidenceType evidenceType, Room *room
// output: void
// input/output: Room *room
// purpose: Adds a evidence of evidence type to room
void addEvidenceInRoom(Ghost *ghost, EvidenceType evidenceType, Room *room){
    addEvidence(evidenceType, ghost->name, &room->droppedEvidence);
}

// input: RoomList *RoomList
// output: void
// input/output: RoomList *RoomList
// purpose: Frees dynamically located memory for all data in roomList
void cleanUpRoomData(RoomList *roomList){
    RoomNode *curNode = roomList->head;
    while (curNode != NULL){
        cleanUpRoomNodes(&curNode->data->connectedRooms);
        cleanUpEvidenceData(&curNode->data->droppedEvidence);
        cleanUpEvidenceNodes(&curNode->data->droppedEvidence);
        cleanUpHunterNodes(&curNode->data->hunters);
        free(curNode->data);
        curNode = curNode->next;
    }
}

// input: RoomList *RoomList
// output: void
// input/output: RoomList *RoomList
// purpose: Frees dynamically located memory for all nodes in roomList
void cleanUpRoomNodes(RoomList *roomList){
    RoomNode *curNode = roomList->head;
    RoomNode *prevNode;
    while(curNode != NULL){
        prevNode = curNode;
        curNode = curNode->next;
        free(prevNode);
    }
}