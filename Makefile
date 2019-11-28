CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic
OBJS = main.o text.o tree_dump.o play_modes.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Debug.
%.g.o: %.cpp
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -c -o $@ $<

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

debug: $(OBJS:.o=.g.o)
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -o $@ $^


.PHONY: clean

clean:
	rm *.o main debug

