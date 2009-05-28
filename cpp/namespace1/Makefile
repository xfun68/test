all : test
        g++ -o test main.o ns1.o ns2.o
main.o : main.cpp
        g++ -c ns1.o
        g++ -c ns2.o
        g++ -c main.o
