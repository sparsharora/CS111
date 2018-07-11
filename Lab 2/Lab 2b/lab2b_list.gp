#! /usr/bin/gnuplot
#
# purpose:
# generate data reduction graphs for the multi-threaded list project
#
# input: lab2b_list.csv
#1. test name
#2. # threads
#3. # iterations per thread
#4. # lists
#5. # operations performed (threads x iterations x (ins + lookup + delete))
#6. run time (ns)
#7. run time per operation (ns)

# general plot parameters
set terminal png
set datafile separator ","

set title "List-1: Throughput of synchronization mechanisms"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Total operations per second for all threads"
set logscale y
set output 'lab2b_1.png'
set key left top
plot \
          "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title '(adjusted) list w/mutex' with linespoints lc rgb 'blue', \
          "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title '(adjusted) list w/spin-lock' with linespoints lc rgb 'green'


set title "List-2: Operation times of mutex synchronization"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Average Time per Operation (in ns)"
set logscale y
set output 'lab2b_2.png'
set key left top
plot \
          "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
          title 'time per operation' with linespoints lc rgb 'blue', \
          "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
          title 'wait for lock' with linespoints lc rgb 'green'


set title "List-3: Threads and Iterations that run without failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Successful Iterations"
set logscale y 10
set output 'lab2b_3.png'
# note that unsuccessful runs should have produced no output
plot \
     "< grep -e 'list-id-none,[0-9]*,[0-9]*,4,' lab2_list.csv" using ($2):($3) \
	title 'w/o sync' with points lc rgb 'green', \
     "< grep -e 'list-id-m,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	title 'w/mutex' with points lc rgb 'blue', \
     "< grep -e 'list-id-s,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	title 'w/spin' with points lc rgb 'violet'

set title "List-4: Throughput of partitioned lists with mutex synchronization"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Aggeregate Throughput"
set logscale y
set output 'lab2b_4.png'
set key left top
plot \
          "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=1' with linespoints lc rgb 'red', \
	  "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=4' with linespoints lc rgb 'violet', \
	  "< grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=8' with linespoints lc rgb 'blue', \
          "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=16' with linespoints lc rgb 'green'

set title "List-5: Throughput of partitioned lists with spin-lock synchronization"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Aggeregate Throughput"
set logscale y
set output 'lab2b_5.png'
set key left top
plot \
          "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=1' with linespoints lc rgb 'red', \
          "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=4' with linespoints lc rgb 'violet', \
          "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=8' with linespoints lc rgb 'blue', \
          "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
          title 'lists=16' with linespoints lc rgb 'green'