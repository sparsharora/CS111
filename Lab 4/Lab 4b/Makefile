#Name: Sparsh Arora
#UID: 804653078
#E-mail: sparsharora@g.ucla.edu


default: lab4b.c
	gcc -o lab4b -lm -lmraa lab4b.c

clean:
	rm -rf *.txt lab4b

dist:
	tar -czvf lab4b-804653078.tar.gz lab4b.c Makefile README testcases.sh

check:
	./testcases.sh
