#!/usr/bin/make -f

all:
	gcc -arch i386 -mtune=i386 -march=i386 -lgobject-2.0.0 -lglib-2.0.0 -lgtk-quartz-2.0.0 -L/Applications/gedit.app/Contents/Resources/lib/ -DHAVE_CONFIG_H -I. `pkg-config --cflags gedit-2.20` src/scrollpaste-plugin.c -Wl,-undefined -Wl,dynamic_lookup -bundle -o libscrollpaste.so
	cp libscrollpaste.so /Applications/gedit.app/Contents/Resources/lib/gedit-2/plugins/

