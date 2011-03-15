AM_CXXFLAGS = @AM_CXXFLAGS@ -I$(top_srcdir)/sfml_headers/include/ -I$(top_srcdir)/Falling -O0 
bin_PROGRAMS = falling_demo

falling_demo_LDADD = $(top_builddir)/Falling/libfalling.a libsfml-system.so libsfml-graphics.so libsfml-window.so $(AM_LDFLAGS)
falling_demo_SOURCES = \
		       affichage.h stdafx.h targetver.h wWidget.h wContener.h wLabel.h wImage.h wDialog.h wMenu.h FallingDemo.cpp pObject.cpp stdafx.cpp wWidget.cpp wContener.cpp wLabel.cpp wImage.cpp wDialog.cpp wMenu.cpp 
