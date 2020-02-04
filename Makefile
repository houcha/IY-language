CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic
OBJS = main.o text.o parser.o node/operators.o node/math_node.o node/node.o node/operator_node.o node/var_node.o


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
main: $(OBJS) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Debug.
%.g.o: %.cpp
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -O0 -c -o $@ $<
debug: $(OBJS:.o=.g.o) $(DEPS)
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -O0 -o $@ $^


.PHONY: clean

clean:
	rm *.o main debug


