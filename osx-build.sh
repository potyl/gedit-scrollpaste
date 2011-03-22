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
	   -lgobject-2.0.0 -lglib-2.0.0 -lgtk-quartz-2.0.0 -L"${TARGET}/lib" \
	   -Wl,-undefined -Wl,dynamic_lookup -bundle \
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
