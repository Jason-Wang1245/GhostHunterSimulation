#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     20000
#define GHOST_WAIT      2400
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef enum LoggerDetails LoggerDetails;
typedef enum ExitReason ExitReason;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };
enum ExitReason { EXIT_BORED, EXIT_FEAR, EXIT_EVIDENCE, EXIT_UNKNOWN };


// Forward typedefs for structures
typedef struct Room Room;
typedef struct RoomNode RoomNode;
typedef struct RoomList RoomList;
typedef struct Ghost Ghost;
typedef struct Hunter Hunter;
typedef struct HunterNode HunterNode;
typedef struct HunterList HunterList;
typedef struct Evidence Evidence;
typedef struct EvidenceNode EvidenceNode;
typedef struct EvidenceList EvidenceList;
typedef struct House House;

// Structures
// LISTS
struct RoomNode {
    Room *data;
    RoomNode *next;
};

struct RoomList {
    RoomNode *head;
    RoomNode *tail;
    int size;
};

struct HunterNode {
    Hunter *data;
    HunterNode *next;
};

struct HunterList {
    HunterNode *head;
    HunterNode *tail;
};

struct EvidenceNode {
    Evidence *data;
    EvidenceNode *next;
};

struct EvidenceList {
    EvidenceNode *head;
    EvidenceNode *tail;
    sem_t mutex;
};
// DATA CONTAINERS
struct Room {
    char *name;
    RoomList connectedRooms;
    EvidenceList droppedEvidence;
    HunterList hunters;
    Ghost *ghost;
    sem_t mutex;
};

struct Ghost {
    char *name;
    GhostClass classType;
    Room *room;
    int boredom;
    ExitReason exitReason;
};

struct Hunter {
    EvidenceType equipment;
    Room *room;
    EvidenceList *evidenceCollection;
    char name[MAX_STR];
    int fear;
    int boredom;
    ExitReason exitReason;
};

struct Evidence {
    EvidenceType evidenceType;
    char *ghostName;
};

struct House {
    HunterList hunters;
    RoomList rooms;
    Ghost *ghost;
    EvidenceList *sharedEvidence;
};

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

// Ghost functions
void initGhost(GhostClass classType, Room *initRoom, char *name, Ghost **ghost);
void incrementGhostBoredom(Ghost *ghost);
int ghostBored(Ghost *ghost);
void resetGhostBoredom(Ghost *ghost);
void moveRoomGhost(Room *room, Ghost *ghost);
void cleanupGhost(Ghost *ghost);
void ghostTypeToEvidenceType(Ghost *ghost, int evidenceTypes[]);

// Hunter functions
void initHunter(EvidenceType equipment, Room *room, EvidenceList *evidenceCollection, char name[], Hunter **hunter);
int incrementHunterFear(Hunter *hunter);
void incrementHunterBoredom(Hunter *hunter);
int hunterBored(Hunter *hunter);
void resetHunterBoredom(Hunter *hunter);
void moveRoomHunter(Room *room, Hunter *hunter);
void initHunterList(HunterList *hunterList);
void addHunter(Hunter *hunter, HunterList *hunterList);
int removeHunter(Hunter *hunter, HunterList *hunterList);
void cleanUpHunterData(HunterList *hunterList);
void cleanUpHunterNodes(HunterList *hunterList);

// Evidence functions
void initEvidence(EvidenceType evidenceType, char *ghostName, Evidence **evidence);
void initEvidenceList(EvidenceList *evidenceList);
int addEvidence(EvidenceType evidenceType, char *ghostName, EvidenceList * evidenceList);
int reviewEvidence(EvidenceList *evidenceList);
void cleanUpEvidenceData(EvidenceList *evidenceList);
void cleanUpEvidenceNodes(EvidenceList *evidenceList);

// Room functions
Room * createRoom(char *name);
void initRoomList(RoomList *roomList);
void addRoom(Room *room, RoomList *roomList);
void connectRooms(Room *roomOne, Room *roomTwo);
void setGhostInRoom(Ghost *ghost, Room *room);
void addHunterInRoom(Hunter *hunter, Room *room);
void removeHunterInRoom(Hunter *hunter, Room *room);
void addEvidenceInRoom(Ghost *ghost, EvidenceType evidenceType, Room *room);
void cleanUpRoomData(RoomList *roomList);
void cleanUpRoomNodes(RoomList *roomList);

// House functions
void populateRooms(House* house);
void initHouse(House *house);
void populateGhost(House *house);
void populateHunters(House *house);
void cleanUpHouse(House *house);

// Thread functions
void *ghostThread(void *data);
void *hunterThread(void *data);

// Results
void printResults(HunterList *hunters, Ghost *ghost, EvidenceList *evidenceList);