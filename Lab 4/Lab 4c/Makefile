#Name: Sparsh Arora
#UID: 804653078
#E-mail: sparsharora@g.ucla.edu


default: 
	gcc -o lab4c_tcp -lm -lmraa -lssl -lcrypt lab4c_tcp.c
	gcc -o lab4c_tls -lm -lmraa -lssl -lcrypt lab4c_tls.c

clean:
	rm -rf *.txt lab4c_tcp lab4c_tls *tar.gz

dist:
	tar -czvf lab4c-804653078.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README

