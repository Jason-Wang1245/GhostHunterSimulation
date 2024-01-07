Name: Jason Wang
Student Number: 101256080
Program Description: Runs simulation of ghost and hunters.
Files:
    Makefile: Makefile for easier compilation (all written by me), includes -Wall -Wextra option for compilation
    defs.h: Forward declarations
    evidence.c: Functions related to evidence and evidenceList, which is a linked list that stores unique evidence (ex. two piece of evidence are not unique if they have the same EvidenceType and ghostName that dropped it)
    ghost.c: Functions related to ghost and ghostList, which is a linked list
    logger.c: Provided logging functions
    main.c: Main control flow of the program
    room.c: Functions associated with room and roomList, which is stored as a linked list
    threads.c: Functions that run the hunter and ghost threads
    utils.c: Provided utility functions
    README.md: project descriptions
Compile Instructions: run "make" (to clear all object files and executable, run "make clean")
Running Instructions: run "./final"
Instructions for using: Results of the simulation will be printed to console. The constant variables in defs.h declared are the same as the ones provided in project specifcations, so they can be changed as expected (though I increase the HUNTER_WAIT and GHOST_WAIT because the logging seemed to have issues)
AI: No AI used for the development of this program.