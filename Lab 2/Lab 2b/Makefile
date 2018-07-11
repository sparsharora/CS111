default: lab2_list

build: lab2_list

lab2_list:
	gcc -pthread -o lab2_list -g SortedList.c -lrt lab2_list.c	
graphs: 
	gnuplot ./lab2b_list.gp
dist:
	tar -czvf lab2b-804653078.tar.gz lab2_list.c SortedList.h SortedList.c profile.out lab2b_list.csv lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png README test2a.sh lab2b_list.gp Makefile

clean:
	rm -f lab2_list *.csv *.png
profile:
	-rm -f ./raw.gperf
	LD_PRELOAD=/u/cs/ugrad/arora/lib/libprofiler.so.0 CPUPROFILE=raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.out
	pprof --list=list_op ./lab2_list ./raw.gperf >> profile.out
	rm -f ./raw.gperf


tests:
	./test2a.sh

	-./lab2_list --threads=1  --iterations=1 --yield=id --lists=4  >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=1  --iterations=2 --yield=id --lists=4  >> lab2b_list.csv  2>/dev/null	
	-./lab2_list --threads=1  --iterations=4 --yield=id --lists=4  >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=1  --iterations=8 --yield=id --lists=4  >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=1  --iterations=16 --yield=id --lists=4  >> lab2b_list.csv  2>/dev/null

	-./lab2_list --threads=4 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=4 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=4 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=4 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=4 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null

	-./lab2_list --threads=8 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=8 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=8 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=8 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=8 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null

	-./lab2_list --threads=12 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=12 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=12 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=12 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=12 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null

	-./lab2_list --threads=16 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null 
	-./lab2_list --threads=16 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=16 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=16 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
	-./lab2_list --threads=16 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv  2>/dev/null
