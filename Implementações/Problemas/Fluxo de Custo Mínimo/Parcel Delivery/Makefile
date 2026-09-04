.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

CXX      = c++
EXT      = cpp
TARGET   = main
INPUT    = input.txt
SRC      = $(TARGET).$(EXT)
CXXFLAGS = -Wl,-z,stack-size=268435456 -std=c++23 -O2
EXE      = ./$(TARGET)
CLIP     = wl-copy
BUNDLE   = python3 ../../../bundle.py

.PHONY: all run clean clip bundle

all: $(EXE) run

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) "$(SRC)" -o "$(EXE)"

run:
	"$(EXE)" < "$(INPUT)"

clean:
	rm -f "$(EXE)"

bundle:
	$(BUNDLE) "$(SRC)"

clip:
	$(BUNDLE) "$(SRC)" | $(CLIP)
