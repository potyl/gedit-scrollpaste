#!/usr/bin/make -f

TARGET=/Applications/gedit.app/Contents/Resources

.PHONY: all
all: plugin libscrollpaste.so


.PHONY: plugin
plugin: configure build install
	@echo "Done"


.PHONY: libscrollpaste.so
libscrollpaste.so: src/scrollpaste-plugin.c
	gcc -arch i386 \
	   -DHAVE_CONFIG_H -I. `pkg-config --cflags gedit-2.20` \
	   -Wl,-undefined -Wl,dynamic_lookup -bunlde \
	   -o $@ $<


.PHONY: configure
configure:
	./configure --prefix="${TARGET}"


.PHONY: build
build:
	make
	make install


.PHONY: install
install: libscrollpaste.so
	cp $< "${TARGET}/lib/gedit-2/plugins/$<"