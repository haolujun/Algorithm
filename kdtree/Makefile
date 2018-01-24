target: main.o kdtree.o
	g++ -o kdtree main.o kdtree.o
kdtree.o:
	g++ -g -c kdtree.cpp
main.o:
	g++ -g -c main.cpp
clean:
	rm -rf kdtree main.o kdtree.o
