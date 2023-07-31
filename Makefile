CFLAGS=-Wall -I./lib
LDFLAGS=-L./lib -llink_core -lutils_modules -lplatform_port -lmiddleware_implementation -Wl,-rpath,'$$ORIGIN/lib'
SOURCES=src/daemon.c src/tuyaConnect.c src/argParser.c src/getSysInfo.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=daemon

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
