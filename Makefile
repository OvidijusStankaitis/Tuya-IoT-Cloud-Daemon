CFLAGS=-Wall -I./lib
LDFLAGS=-L./lib -llink_core -lutils_modules -lplatform_port -lmiddleware_implementation -Wl,-rpath,'$$ORIGIN/lib'
SOURCES=src/main.c src/tuyaConnect.c src/argParser.c src/getSysInfo.c src/daemon.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tuya_cloud_daemon

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

PREFIX=/usr/local

install-lib:
	install -m 644 lib/*.h $(PREFIX)/include
	install -m 644 lib/*.so $(PREFIX)/lib
	ldconfig

uninstall-lib:
	for file in $(wildcard lib/*.h); do $(RM) $(PREFIX)/include/`basename $$file`; done
	for file in $(wildcard lib/*.so); do $(RM) $(PREFIX)/lib/`basename $$file`; done
	ldconfig

.PHONY: all clean install-lib uninstall-lib
