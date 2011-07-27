all : main servepositions

.PHONY : all

main: main.cpp
	g++ main.cpp -o main `pkg-config --libs --cflags opencv cvblob`

servepositions : servepositions.cpp
	g++ servepositions.cpp -o servepositions `pkg-config --libs --cflags opencv cvblob`
