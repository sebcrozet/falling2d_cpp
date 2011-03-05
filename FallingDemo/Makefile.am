AM_CXXFLAGS = -I$(top_srcdir)/sfml_headers/include/ -I$(top_srcdir)/Falling @AM_CXXFLAGS@
bin_PROGRAMS = falling_demo

falling_demo_LDADD = $(top_builddir)/Falling/libfalling.a libsfml-system.so libsfml-graphics.so libsfml-window.so $(AM_LDFLAGS)
falling_demo_SOURCES = \
		       affichage.h stdafx.h targetver.h wBase.h wDialog.h wMenu.h wRectangularBase.h FallingDemo.cpp pObject.cpp stdafx.cpp wBase.cpp wDialog.cpp wMenu.cpp 
