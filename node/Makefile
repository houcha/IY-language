CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic
OBJS = node.o math_node.o var_node.o operator_node.o create_node.o operators.o


all: generate $(OBJS)
	@echo Node project is built.

generate: operator_gen.py
	python operator_gen.py

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


debug: generate $(OBJS:.o=.g.o)
	@echo Node debug project is built.

# Debug.
%.g.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -c -o $@ $<


.PHONY: clean

clean:
	rm *.o

