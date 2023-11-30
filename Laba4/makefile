main: libfunctions1.so libfunctions2.so
	gcc Static.c -L. -lfunctions1 -o main1.1 -lm -Wl,-rpath -Wl,.
	gcc Static.c -L. -lfunctions2 -o main1.2 -lm -Wl,-rpath -Wl,.
	gcc Dynamic.c -L. -ldl -lm -o main2 -Wl,-rpath -Wl,.

libfunctions1.so libfunctions2.so: functions1.o functions2.o
	gcc -shared -o libfunctions1.so functions1.o
	gcc -shared -o libfunctions2.so functions2.o

functions1.o functions2.o: func1.c func2.c
	gcc -fPIC -c func1.c -lm -o functions1.o
	gcc -fPIC -c func2.c -lm -o functions2.o

