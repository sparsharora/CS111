#Sparsh Arora
#UID:804653078	
#sparsharora@g.ucla.edu

default: 
	gcc -o lab1b-client -lmcrypt lab1b-client.c
	gcc -o lab1b-server -lmcrypt lab1b-server.c

lab1b-client:
	gcc -o -lmcrypt lab1b-client lab1b-client.c

lab1b-sever:
	gcc -o -lmcrypt lab1b-server lab1b-server.c

clean:
	rm -f lab1b-client lab1b-server lab1b*tar.gz

dist:
	tar -czf lab1b-804653078.tar.gz README Makefile lab1b-server.c lab1b-client.c my.key
