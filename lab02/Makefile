.phony: All Clean

All: lab02

lab02: lab2.c
			gcc lab2.c -o lab2
Debug: lab2.c
		gcc -g lab2.c -o lab2.o
		valgrind --leak-check=yes ./lab2.o ./ ./

Clean: rm -f lab2 lab2.o
