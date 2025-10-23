# Makefile

# Fórmulas que serão construídas quando o comando "make" for enviado no terminal
all: test_acc test_store test_load  matrix_gen test_prof

test_acc: adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_acc.o
	gcc adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_acc.o -o test_acc -lm -fopenmp

test_acc.o: test_acc.c
	gcc -c test_acc.c -o test_acc.o -fopenmp

test_store: adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_store.o
	gcc adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_store.o -o test_store -lm -fopenmp

test_store.o: test_store.c
	gcc -c test_store.c -o test_store.o -fopenmp

test_load: adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_load.o
	gcc adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_load.o -o test_load -lm -fopenmp

test_load.o: test_load.c
	gcc -c test_load.c -o test_load.o -fopenmp

test_prof: adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_prof.o
	gcc adjlist.o str_reduc.o heap_graph.o mst.o dequef.o timer.o test_prof.o -o test_prof -lm -fopenmp

test_prof.o: test_prof.c
	gcc -c test_prof.c -o test_prof.o -fopenmp -lm

matrix_gen: adjlist.o str_reduc.o heap_graph.o mst.o dequef.o matrix_gen.o
	gcc adjlist.o str_reduc.o heap_graph.o mst.o dequef.o matrix_gen.o -o matrix_gen -lm -fopenmp

matrix_gen.o: matrix_gen.c
	gcc -c matrix_gen.c -o matrix_gen.o -lm

adjlist.o: adjlist.c
	gcc -c adjlist.c -o adjlist.o -fopenmp

str_reduc.o: str_reduc.c
	gcc -c str_reduc.c -o str_reduc.o -fopenmp

heap_graph.o: heap_graph.c
	gcc -c heap_graph.c -o heap_graph.o -fopenmp

mst.o: mst.c
	gcc -c mst.c -o mst.o -fopenmp

dequef.o: dequef.c
	gcc -c dequef.c -o dequef.o -fopenmp

timer.o: timer.c timer.h timer_macros.h
	gcc -c timer.c -o timer.o

clean:
	rm *.o
	rm test_acc
	rm test_store
	rm test_load
	rm test_prof
	rm matrix_gen