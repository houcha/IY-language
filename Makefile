CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic
OBJS = main.o text.o expression_tree.o
SUBDIR = node


all: make_subdir $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(wildcard node/*.o)

make_subdir:
	$(MAKE) -C $(SUBDIR) all

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


debug: make_subdir_debug $(OBJS:.o=.g.o)
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -o $@ $(OBJS:.o=.g.o) $(wildcard node/*.g.o)

make_subdir_debug:
	$(MAKE) -C $(SUBDIR) debug

objects_debug:
	$(CXX) $(CXXFLAGS) -g -o $@ $^

main.g.o: main.cpp
	$(CXX) $(CXXFLAGS) -g -c -o $@ $<
%.g.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -c -o $@ $<


.PHONY: clean

clean:
	-$(MAKE) -C $(SUBDIR) clean
	-rm *.o all debug

