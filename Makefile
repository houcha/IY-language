CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic



SRC = main.cpp

all:
	$(CXX) $(CXXFLAGS) -o all.exe $(SRC)
	./all.exe



debug:
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -o debug.exe $(SRC)
	gdb debug.exe --tui



TEST_SRC = test_stack.cpp
# Add -lgtest_main -lpthread flags if test target do not compile.
GTEST_FLAGS = -lgtest
GTEST_PATH = /usr/include/gtest/ -L /usr/lib/

test:
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -o test.exe $(TEST_SRC) -I $(GTEST_PATH) $(GTEST_FLAGS)
	./test.exe



clean:
	rm run.exe debug.exe test.exe

