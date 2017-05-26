CC := g++
Flags := -std=c++11
CUnit := -lcunit
gcov := --coverage
gcov2 := -fprofile-arcs -ftest-coverage
gcov_options := --function-summaries

StateTable := StateTable.cpp
Timer := Timer.cpp
Buffer := Buffer.cpp

test:
	$(CC) test.cpp $(Flags) $(CUnit) $(gcov2) $(StateTable) $(Timer) $(Buffer) -o test 
	
gcov:
	./test
	gcov $(gcov_options) test.cpp
	
clean:
	rm -rf test *.o *.gcno *gcda *.gcov
	
all: clean test