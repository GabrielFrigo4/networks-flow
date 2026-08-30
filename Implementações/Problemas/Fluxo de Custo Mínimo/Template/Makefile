CXX = c++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pedantic

TARGET = main
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) < input.txt

.PHONY: all clean run
