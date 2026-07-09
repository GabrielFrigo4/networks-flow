CXX    := c++
TARGET := segment
SRC    := main.cpp

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

SAMPLES := $(wildcard samples/*/.)
PASS    := \033[0;32m[PASS]\033[0m
FAIL    := \033[0;31m[FAIL]\033[0m

.PHONY: all run test example clean

all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXE)

run: $(EXE)
	$(EXEC_CMD)

test: $(EXE)
	@FAILED=0; \
	for dir in samples/*/; do \
		name=$$(basename $$dir); \
		input=$$dir$$name.ppm; \
		seeds=$$dir$$name.seeds; \
		output=$$dir/output.ppm; \
		if $(EXEC_CMD) --input $$input --seeds $$seeds --output $$output > /dev/null 2>&1; then \
			printf "$(PASS) $$name\n"; \
		else \
			printf "$(FAIL) $$name\n"; \
			FAILED=$$((FAILED+1)); \
		fi; \
	done; \
	exit $$FAILED

example: $(EXE)
	$(EXEC_CMD) --input samples/grid/grid.ppm --seeds samples/grid/grid.seeds --output samples/grid/output.ppm

clean:
	-$(RM_CMD)
