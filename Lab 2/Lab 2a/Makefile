default: lab2_add lab2_list

build: lab2_add lab2_list
lab2_add:
	gcc -o  lab2_add -pthread -lrt lab2_add.c
lab2_list:
	gcc -pthread -o lab2_list -g SortedList.c -lrt lab2_list.c	
graphs: 
	gnuplot ./lab2_add.gp
	gnuplot ./lab2_list.gp
dist:
	tar -czvf lab2a-804653078.tar.gz lab2_add.c lab2_list.c SortedList.h SortedList.c lab2_add.csv lab2_list.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png README test.sh lab2_add.gp lab2_list.gp Makefile

clean:
	rm -f lab2_add lab2_list *.csv *.png


tests:
	./test.sh
	-./lab2_list --threads=2  --iterations=1              >> lab2_list.csv  2>/dev/null
	-./lab2_list --threads=2  --iterations=10             >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=100            >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=1000           >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=1              >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=10             >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=100            >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=1000           >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=1              >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=10             >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=100            >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=1000           >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=1              >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=10             >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=100            >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=1000           >> lab2_list.csv	2>/dev/null

	-./lab2_list --threads=2  --iterations=1   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=2   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=4   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=8   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=16  --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=32  --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=1   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=2   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=4   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=8   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=16  --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=1   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=2   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=4   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=8   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=16  --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=1   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=2   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=4   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=8   --yield=i  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=16  --yield=i  >> lab2_list.csv	2>/dev/null

	-./lab2_list --threads=2  --iterations=1   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=2   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=4   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=8   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=16  --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=32  --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=1   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=2   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=4   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=8   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=16  --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=1   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=2   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=4   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=8   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=16  --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=1   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=2   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=4   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=8   --yield=d  >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=16  --yield=d  >> lab2_list.csv	2>/dev/null

	-./lab2_list --threads=2  --iterations=1   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=2   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=4   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=8   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=16  --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=32  --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=1   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=2   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=4   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=8   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=16  --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=1   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=2   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=4   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=8   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=16  --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=1   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=2   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=4   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=8   --yield=il >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=16  --yield=il >> lab2_list.csv	2>/dev/null

	-./lab2_list --threads=2  --iterations=1   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=2   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=4   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=8   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=16  --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=2  --iterations=32  --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=1   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=2   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=4   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=8   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=4  --iterations=16  --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=1   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=2   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=4   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=8   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=8  --iterations=16  --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=1   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=2   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=4   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=8   --yield=dl >> lab2_list.csv	2>/dev/null
	-./lab2_list --threads=12 --iterations=16  --yield=dl >> lab2_list.csv	2>/dev/null
