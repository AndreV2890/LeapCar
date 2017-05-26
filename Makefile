CC := g++
LIBRARY := 
FLAGS = -std=c++11

PTHREAD_OPT := -lpthread -lrt

MY_THREAD := threads_managing/my_thread_lib.c
LEAP_LIB := leap/lib/x64/libLeap.so -Wl,-rpath,leap/lib/x64
FILE_CPP := leap/function_leapmotion.cpp leap/MyCarMotion.cpp
SERIALE := serial/DiViSeriale.cpp serial/DiViClientSeriale.cpp 
BUFFER := serial/Buffer.cpp
TIMER := serial/Timer.cpp
STATEMACHINE_CPP := serial/StateTable.cpp

all: clean CarMotion

CarMotion: 
	$(CC) -g main.cpp $(FLAGS) $(MY_THREAD) $(FILE_CPP) $(STATEMACHINE_CPP) $(SERIALE) $(BUFFER) \
	$(TIMER) -o CarMotion $(LEAP_LIB) $(PTHREAD_OPT) -L. $(LEAP_LIB) 

clean:
	rm -rf CarMotion *.o 
