TARGETS=evidence.o ghost.o house.o hunter.o logger.o room.o utils.o main.o threads.o
OPT=-Wall -Wextra -Werror

final: ${TARGETS}
	gcc ${OPT} -o final ${TARGETS} -pthread

main.o: main.c defs.h
	gcc ${OPT} -c main.c -pthread

evidence.o: evidence.c defs.h
	gcc ${OPT} -c evidence.c -pthread

ghost.o: ghost.c defs.h
	gcc ${OPT} -c ghost.c -pthread

house.o: house.c defs.h
	gcc ${OPT} -c house.c -pthread

hunter.o: hunter.c defs.h
	gcc ${OPT} -c hunter.c -pthread

logger.o: logger.c defs.h
	gcc ${OPT} -c logger.c -pthread

room.o: room.c defs.h
	gcc ${OPT} -c room.c -pthread

utils.o: utils.c defs.h
	gcc ${OPT} -c utils.c -pthread

threads.o: threads.c defs.h
	gcc ${OPT} -c threads.c -pthread

clean:
	rm -f ${TARGETS} final