CXX    := g++
EXT    := cpp
TARGET := segment
SRC    := main.$(EXT)

ifeq ($(OS),Windows_NT)
	CXXFLAGS := -Wl,--stack=268435456 -std=c++23 -O2
	EXE      := $(TARGET).exe
	RM_CMD   := del /f /q $(EXE) 2>nul || rm -f $(EXE)
	EXEC_CMD := ./$(EXE)
else
	CXXFLAGS := -Wl,-z,stack-size=268435456 -std=c++23 -O2
	EXE      := ./$(TARGET)
	RM_CMD   := rm -f $(EXE)
	EXEC_CMD := $(EXE)
endif

.PHONY: all run clean example

all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXE)

run: $(EXE)
	$(EXEC_CMD)

example: $(EXE)
	$(EXEC_CMD) --input test_images/simple_10x10.ppm --seeds test_images/simple_10x10_seeds.txt --output output_segmented.ppm

clean:
	-$(RM_CMD)
