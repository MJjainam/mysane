AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = myscan

#AM_CPPFLAGS = ${AM_CPPFLAGS}
AM_CPPFLAGS += -I/home/radix/Desktop/cprogs/include/  //Adds where to search from the files in #include "*.h"

myscan_LDADD = -ljpeg  //to add jpeglib.h library

myscan_SOURCES = ./frontend/myscan.c ./backend/plustek_pp.c // what all to compile and link
