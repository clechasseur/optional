VPATH = lib/cl:lib/cl/optional:tests:tests/coveo:tests/cl:tests/cl/optional

all_tests.out: all_tests.o optional_tests.o tests_main.o
	$(CXX) -o all_tests.out all_tests.o optional_tests.o tests_main.o

all_tests.o: all_tests.cpp all_tests.h
	$(CXX) -c -std=$(STD) tests/cl/optional/all_tests.cpp -Ilib -Itests
optional_tests.o: optional_tests.cpp optional_tests.h optional.h
	$(CXX) -c -std=$(STD) tests/cl/optional/optional_tests.cpp -Ilib -Itests
tests_main.o: tests_main.cpp
	$(CXX) -c -std=$(STD) tests/tests_main.cpp -Ilib -Itests

clean:
	rm all_tests.out all_tests.o optional_tests.o tests_main.o

