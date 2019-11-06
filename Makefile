CXX_FLAGS = -std=c++17
CHECK_FLAGS = -Wall -pedantic

SUBDIR = src


generate_code:
	python $(SUBDIR)/command_generator.py


%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

$(SUBDIR)/%.o: $(SUBDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS)


compile: compile.o\
$(SUBDIR)/assembler.o $(SUBDIR)/text.o
	$(CXX) -o $@ $^ $(CXX_FLAGS)

run: run.o\
$(SUBDIR)/processor.hpp $(SUBDIR)/text.o $(SUBDIR)/disassembler.o
	$(CXX) -o $@ $^ $(CXX_FLAGS)

disassembly: disassembly.o\
$(SUBDIR)/disassembler.o $(SUBDIR)/text.o
	$(CXX) -o $@ $^ $(CXX_FLAGS)


build: generate_code compile run disassembly
	echo


debug_run: generate_code run.o\
$(SUBDIR)/processor.hpp $(SUBDIR)/text.o $(SUBDIR)/disassembler.o
	$(CXX) -o $@ -g $^ $(CXX_FLAGS) $(CHECK_FLAGS)


.PHONY: clean

clean:
	find . -type f -name '*.o' -delete
	rm compile run disassembly debug_run

