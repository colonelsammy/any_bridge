CC=g++
CFLAGS=-c -Wall -I../include
LDFLAGS=
SOURCES=main.cpp \
	bridge_example.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=example

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE).exe

