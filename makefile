project1b: ./src/project2.c ./src/memory_algos/firstfit.c ./src/memory_algos/nextfit.c ./src/memory_algos/bestfit.c ./src/memory_algos/buddysystem.c
	gcc -o project2.out ./src/project2.c ./src/memory_algos/firstfit.c ./src/memory_algos/nextfit.c ./src/memory_algos/bestfit.c ./src/memory_algos/buddysystem.c -I.
clean: 
	rm -f project2.out
all:
	gcc -o project2.out ./src/project2.c ./src/memory_algos/firstfit.c ./src/memory_algos/nextfit.c ./src/memory_algos/bestfit.c ./src/memory_algos/buddysystem.c -I.