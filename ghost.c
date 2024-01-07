#include "defs.h"

// input: GhostClass classType, Room *initRoom, char *name, Ghost **ghost
// output: void
// input/output: Ghost **ghost
// purpose: Intializes values of ghost with provided parameters and dynamically allocating space for ghost and setting the pointer to it
void initGhost(GhostClass classType, Room *initRoom, char *name, Ghost **ghost){
    Ghost *ghostPointer = (Ghost *) calloc(1, sizeof(Ghost));
    if (ghostPointer == NULL){
        printf("Memory Allocation Error.\n");
        return;
    }
    ghostPointer->name = name;
    ghostPointer->classType = classType;
    ghostPointer->room = initRoom;
    ghostPointer->boredom = 0;
    ghostPointer->exitReason = EXIT_UNKNOWN;
    *ghost = ghostPointer;
}

// input: Ghost *ghost
// output: void
// input/output: Ghost *ghost
// purpose: Increments the input ghosts boredom by 1
void incrementGhostBoredom(Ghost *ghost){
    ghost->boredom = ghost->boredom + 1;
}

// input: Ghost *ghost
// output: int (boolean value)
// input/output: n/a
// purpose: Checks if a ghosts boredom value has reached BOREDOM_MAX, if so return C_TRUE, return C_FALSE otherwise
int ghostBored(Ghost *ghost){
    return ghost->boredom >= BOREDOM_MAX;
}

// input: Ghost *ghost
// output: void
// input/output: Ghost *ghost
// purpose: Resets the input ghosts boredom value to 0
void resetGhostBoredom(Ghost *ghost){
    ghost->boredom = 0;
}

// input: Room *room, Ghost *ghost
// output: void
// input/output: Ghost *ghost
// puropose: Changes the ghost's room to the provided input room
void moveRoomGhost(Room *room, Ghost *ghost){
    ghost->room = room;
}

// input: Ghost *ghost
// output: void
// input/output: Ghost *ghost
// purpose: Frees memory allocated by ghost input
void cleanupGhost(Ghost *ghost){
    free(ghost);
}

// input: Ghost *ghost, Ghost *ghost, int evidenceTypes[]
// output: void
// input/output: Ghost *ghost, int evidenceTypes[]
// purpose: Sets the evidenceTypes array to elements that only the ghost based on its type classType can drop
void ghostTypeToEvidenceType(Ghost *ghost, int evidenceTypes[]){
    if (ghost->classType == POLTERGEIST){
        evidenceTypes[0] = EMF;
        evidenceTypes[1] = TEMPERATURE;
        evidenceTypes[2] = FINGERPRINTS;
    } else if (ghost->classType == BANSHEE){
        evidenceTypes[0] = EMF;
        evidenceTypes[1] = TEMPERATURE;
        evidenceTypes[2] = SOUND;
    } else if (ghost->classType == BULLIES){
        evidenceTypes[0] = EMF;
        evidenceTypes[1] = FINGERPRINTS;
        evidenceTypes[2] = SOUND;
    } else if (ghost->classType == PHANTOM){
        evidenceTypes[0] = TEMPERATURE;
        evidenceTypes[1] = FINGERPRINTS;
        evidenceTypes[2] = SOUND;
    }
}