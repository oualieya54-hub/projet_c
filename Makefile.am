bin_PROGRAMS = project4

project4_SOURCES = \
	main.c \
	callbacks.c callbacks.h \
	interface.c interface.h \
	support.c support.h \
	project4.c project4.h

project4_CFLAGS = $(PACKAGE_CFLAGS)
project4_LDADD = $(PACKAGE_LIBS)

AM_CPPFLAGS = -DPACKAGE_DATA_DIR=\""$(datadir)"\"
