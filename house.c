#include "defs.h"

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(House* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(van, &house->rooms);
    addRoom(hallway, &house->rooms);
    addRoom(master_bedroom, &house->rooms);
    addRoom(boys_bedroom, &house->rooms);
    addRoom(bathroom, &house->rooms);
    addRoom(basement, &house->rooms);
    addRoom(basement_hallway, &house->rooms);
    addRoom(right_storage_room, &house->rooms);
    addRoom(left_storage_room, &house->rooms);
    addRoom(kitchen, &house->rooms);
    addRoom(living_room, &house->rooms);
    addRoom(garage, &house->rooms);
    addRoom(utility_room, &house->rooms);
}

// input: House *house
// output: void
// input/output House *house
// purpose: Initializes attributes for house
void initHouse(House *house){
    EvidenceList *evidenceListPointer = (EvidenceList *) calloc(1, sizeof(EvidenceList));
    if (evidenceListPointer == NULL){
        printf("Memory Allocation Error.\n");
        return;
    }
    initHunterList(&house->hunters);
    initRoomList(&house->rooms);
    house->ghost = NULL;
    initEvidenceList(evidenceListPointer);
    house->sharedEvidence = evidenceListPointer;
}

// input: House *house
// output: void
// input/output: House *house
// purpose: Adds a ghost to house
void populateGhost(House *house){
    Ghost *ghost;
    RoomNode *curNode = house->rooms.head->next;
    for (int i = 0; i < randInt(0, house->rooms.size - 1); i++){
        curNode = curNode->next;
    }
    initGhost(randInt(0, GHOST_COUNT), curNode->data, "Ghost", &ghost);
    house->rooms.head->data->ghost = ghost;
    house->ghost = ghost;
    l_ghostInit(ghost->classType, ghost->room->name);
    house->ghost = ghost;
}

// input: House *house
// output: void
// input/output: House *house
// purpose: Adds a hunters to house, also gets user input on names for hunters
void populateHunters(House *house){
    for (int i = 0; i < NUM_HUNTERS; i++){
        Hunter *hunter;
        char name[MAX_STR];
        printf("Enter the name of hunter %d: ", i + 1);
        scanf("%s", name);
        while(getchar() != '\n');
        initHunter(i, house->rooms.head->data, house->sharedEvidence, name, &hunter);
        addHunter(hunter, &house->hunters);
        l_hunterInit(hunter->name, hunter->equipment);
    }
}

// input: House *house
// output: void
// input/output: House *house
// purpose: Free up all dynamically allocated memory in house
void cleanUpHouse(House *house){
    cleanupGhost(house->ghost);
    cleanUpHunterData(&house->hunters);
    cleanUpHunterNodes(&house->hunters);
    cleanUpRoomData(&house->rooms);
    cleanUpRoomNodes(&house->rooms);
    cleanUpEvidenceData(house->sharedEvidence);
    cleanUpEvidenceNodes(house->sharedEvidence);
    free(house->sharedEvidence);
}