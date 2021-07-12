CC = gcc
H = io.h types.h transaction.h
C = main.c io.c transaction.c
EXEC = a6

build: ${C} ${H}
	${CC} ${C} -o ${EXEC} -lpthread

clean:
	@echo "Cleaning up..."
	rm -f ${EXEC} output_file.txt
