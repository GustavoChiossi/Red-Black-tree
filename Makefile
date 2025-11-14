myrb: myrb.cpp
	g++ myrb.cpp -o myrb
all: myrb
clean:
	rm -f myrb teste.out
run: myrb
	./myrb < palavras.txt > teste.out
