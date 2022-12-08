life:
	g++ life.cpp -o life -lncurses -lpthread

clean:
	rm -f life life.o
