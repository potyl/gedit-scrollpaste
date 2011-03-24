#!/usr/bin/make -f

all:
	CFLAGS="-arch i386" ./configure --prefix=/Applications/gedit.app/Contents/Resources
	make
	make install

old:
	gcc -arch i386 -mtune=i386 -march=i386 \
	  -DHAVE_CONFIG_H -I. `pkg-config --cflags gedit-2.20` \
	  -lgobject-2.0.0 -lglib-2.0.0 -lgtk-quartz-2.0.0 -L/Applications/gedit.app/Contents/Resources/lib/ \
	  -Wl,-undefined -Wl,dynamic_lookup -bundle \
	  -o libscrollpaste.so src/scrollpaste-plugin.c
	cp libscrollpaste.so /Applications/gedit.app/Contents/Resources/lib/gedit-2/plugins/
