CFLAGS=-Wall -I./lib
LDFLAGS=-L./lib -llink_core -lutils_modules -lplatform_port -lmiddleware_implementation -Wl,-rpath,'$$ORIGIN/lib'
SOURCES=src/main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: setlib $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
