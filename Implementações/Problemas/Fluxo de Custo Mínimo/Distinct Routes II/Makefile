.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory -s

CXX         = c++
EXT         = cpp
TARGET      = main
INPUT       = input.txt
SRC         = $(TARGET).$(EXT)

STACK_FLAG != case "$$(uname -s 2> /dev/null)" in *Windows*|*MINGW*|*MSYS*|*CYGWIN*) echo "-Wl,--stack,268435456" ;; *) echo "-Wl,-z,stack-size=268435456" ;; esac
EXE_EXT    != case "$$(uname -s 2> /dev/null)" in *Windows*|*MINGW*|*MSYS*|*CYGWIN*) echo ".exe" ;; *) echo "" ;; esac
CXXFLAGS    = $(STACK_FLAG) -std=c++23 -O2

EXE         = ./$(TARGET)$(EXE_EXT)

CLIP        = wl-copy
BUNDLE      = python3 ../../../bundle.py

.PHONY: all run clean clear clip bundle

all: $(EXE) run

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) "$(SRC)" -o "$(EXE)"

run:
	"$(EXE)" < "$(INPUT)"

clean clear:
	rm -f "$(EXE)" "$(EXE).exe"

bundle:
	$(BUNDLE) "$(SRC)"

clip:
	$(BUNDLE) "$(SRC)" | $(CLIP)
