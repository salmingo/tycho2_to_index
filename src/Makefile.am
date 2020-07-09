bin_PROGRAMS=tycho2index
tycho2index_SOURCES=FITSHandler.hpp ATimeSpace.cpp build_index.cpp tycho2index.cpp

if DEBUG
  AM_CFLAGS = -g3 -O0 -Wall -DNDEBUG
  AM_CXXFLAGS = -g3 -O0 -Wall -DNDEBUG
else
  AM_CFLAGS = -O3 -Wall
  AM_CXXFLAGS = -O3 -Wall
endif

tycho2index_LDFLAGS = -L/usr/local/lib
tycho2index_LDADD = -lm -lcfitsio -lboost_system-mt
