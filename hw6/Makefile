.PHONY: All

All: q16 q17 hello-hw dotpr-hw

q16: q16.c csapp.c csapp.h
		gcc csapp.c q16.c -o q16.o -pthread
q17: q17.c csapp.c csapp.h
		gcc csapp.c q17.c -o q17.o -pthread

hello-hw: hellow-hw.c csapp.c csapp.h
		gcc csapp.c hello-hw.c -o hellow-hw.o -pthread

dotpr-hw: dotpr-hw.c csapp.c csapp.h
		gcc csapp.c dotpr-hw.c -o dotpr-hw.o -pthread

