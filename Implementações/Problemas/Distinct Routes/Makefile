CXX    := g++
EXT    := cpp
TARGET := main
INPUT  := input.txt
SRC    := $(TARGET).$(EXT)

ifneq (,$(findstring MINGW,$(UNAME_S))$(findstring MSYS,$(UNAME_S))$(filter Windows_NT,$(OS)))
	CXXFLAGS := -Wl,--stack=268435456 -std=c++23 -O2
	EXE      := $(TARGET).exe
	CLIP     := win32yank -i
	RM_CMD   := del /f /q $(EXE) 2>nul || rm -f $(EXE)
	EXEC_CMD := $(EXE)
else
	CXXFLAGS := -Wl,-z,stack-size=268435456 -std=c++23 -O2
	EXE      := ./$(TARGET)
	CLIP     := wl-copy
	RM_CMD   := rm -f $(EXE)
	EXEC_CMD := $(EXE)
endif

.PHONY: all run clean clip

all: $(EXE) run

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXE)

run:
	$(EXEC_CMD) < $(INPUT)

clean:
	-$(RM_CMD)

clip:
	$(CLIP) < $(SRC)
