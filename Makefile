assy: main.o commands.o line_cost.o file_to_array.o file_memory_mgmt.o first_transition.o second_transition.o
	gcc -o assy main.o commands.o line_cost.o file_to_array.o file_memory_mgmt.o first_transition.o second_transition.o -Wall -ansi -pedantic -lm

main.o: main.c main.h data.h
	gcc -c main.c -Wall -ansi -pedantic -lm

commands.o: commands.c commands.h
	gcc -c commands.c -Wall -ansi -pedantic -lm

line_cost.o: line_cost.c line_cost.h
	gcc -c line_cost.c -Wall -ansi -pedantic -lm

file_to_array.o: file_to_array.c file_to_array.h
	gcc -c file_to_array.c -Wall -ansi -pedantic -lm

file_memory_mgmt.o: file_memory_mgmt.c file_memory_mgmt.h
	gcc -c file_memory_mgmt.c -Wall -ansi -pedantic -lm

first_transition.o: first_transition.c first_transition.h
	gcc -c first_transition.c -Wall -ansi -pedantic -lm

second_transition.o: second_transition.c second_transition.h
	gcc -c second_transition.c -Wall -ansi -pedantic -lm

clean:
	rm -f *.o all
