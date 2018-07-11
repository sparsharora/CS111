#!/bin/bash
let errors=0
let cases=0

EXIT_OK=0
EXIT_BADIN=1

./lab4b --periods > /dev/null 2> /dev/null
if [ $? -eq $EXIT_BADIN ]
then
   let cases+=1
   echo "Passed test 1: Code runs succesfully with right exit code"
else
    let errors+=1
    echo "Error1: Wrong exit code for bad input"
fi

./lab4b --scale=D >/dev/null  2> /dev/null 
		
if [ $? -eq $EXIT_BADIN ]
then
    let cases+=1
    echo "Passed test 2: Code runs succesfully with right exit codes for wrong arguments"
else
   let errors+=1
   echo "Error2: Wrong exit code for wrong format of arguments"
fi

./lab4b --period=2 --log=test.txt > /dev/null 2> /dev/null <<-EOF
STOP
OFF
EOF

if [ $? -eq $EXIT_OK ]
then
    let cases+=1
    echo "Passed test 3: Code runs succesfully with right exit codes for bad/non existing input arguments"
else
    let errors+=1
    echo "Error3: Wrong exit code for good input"
fi

./lab4b --period=2 --log=test.txt > /dev/null 2> /dev/null <<-EOF
STOP
START
SCALE=C
PERIOD=3
OFF
EOF

if [ -s test.txt ]
then 
	let cases+=1
	echo "Passed test 4: Code succesfully creates log file!"
else
	let errors+=1
	echo "Error 4: Code doesn't create log file!"
fi


./lab4b --period=2 --log=test.txt > /dev/null 2> /dev/null <<-EOF
STOP
START
SCALE=C
PERIOD=3
OFF
EOF

for c in STOP START SCALE=C PERIOD=3 OFF SHUTDOWN
do
	
	grep $c test.txt > /dev/null
	if [ $? -ne 0 ]
	then
		echo "Error 5: DID NOT LOG $c command"
		let errors+=1
	fi
										done

										./lab4b --p=2 --l=test.txt > /dev/null 2> /dev/null <<-EOF
STOP
START										OFF
EOF

egrep '[0-9][0-9].[0-9]' test.txt > /dev/null
if [ $? -eq 0 ] 
then
		echo "Passed Case 6: Temperature sensor detected"
		let cases+=1
else
		echo "Error 6: Temperature sensor not found"
		let errors+=1
fi

if [ $errors -eq 0 ]
then
	echo "GOOD! All $cases tests passed"
else
	echo "Failed with $errors errors!"
fi
