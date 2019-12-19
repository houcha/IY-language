CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic
OBJS = main.o text.o grammar_parser.o
DEPS =


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
main: $(OBJS) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Debug.
%.g.o: %.cpp
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -c -o $@ $<
debug: $(OBJS:.o=.g.o) $(DEPS)
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -o $@ $^


.PHONY: clean

clean:
	rm *.o main debug


