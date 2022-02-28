CXX_FLAGS = -std=c++17
CHECK_FLAGS = -Wall -pedantic

SRC_DIR = src
OBJ_DIR = obj


generate_code:
	python3 $(SRC_DIR)/command_generator.py


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS)


compile: $(OBJ_DIR)/compile.o\
$(OBJ_DIR)/assembler.o $(OBJ_DIR)/text.o
	$(CXX) -o $@ $^ $(CXX_FLAGS)

run: $(OBJ_DIR)/run.o\
$(SRC_DIR)/processor.hpp $(OBJ_DIR)/text.o $(OBJ_DIR)/disassembler.o
	$(CXX) -o $@ $^ $(CXX_FLAGS)

disassembly: $(OBJ_DIR)/disassembly.o\
$(OBJ_DIR)/disassembler.o $(OBJ_DIR)/text.o
	$(CXX) -o $@ $^ $(CXX_FLAGS)


build: make_obj_dir generate_code compile run disassembly
	@echo Build completed


debug_run: generate_code run.o\
$(SRC_DIR)/processor.hpp $(SRC_DIR)/text.o $(SRC_DIR)/disassembler.o
	$(CXX) -o $@ -g $^ $(CXX_FLAGS) $(CHECK_FLAGS)

make_obj_dir:
	mkdir -p $(OBJ_DIR)


.PHONY: clean

clean:
	rm -r $(OBJ_DIR)
	-rm compile run disassembly debug_run

