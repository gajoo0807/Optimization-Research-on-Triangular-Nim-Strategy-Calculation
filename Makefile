all: backward game retro

game: game.cpp
	g++ game.cpp -o game.out -std=c++11 -O3

retro: retro.cc
	g++ retro.cc -o retro -O3 -fopenmp

backward: backward.cc
	g++ backward.cc -o backward -O3 -pthread

clean:
	rm -f main testsort visual game