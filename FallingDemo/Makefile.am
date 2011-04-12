AM_CXXFLAGS = @AM_CXXFLAGS@ -I$(top_srcdir)/sfml_headers/include/ -I$(top_srcdir)/Falling -O2 -Wall -Wextra -Werror
bin_PROGRAMS = falling_demo

falling_demo_LDADD = $(top_builddir)/Falling/libfalling.a /usr/lib/libsfml-system.so /usr/lib/libsfml-graphics.so /usr/lib/libsfml-window.so $(AM_LDFLAGS)
falling_demo_SOURCES = \
		       stdafx.h targetver.h State.h UserInterface.h wWidget.h wContener.h wLabel.h wImage.h wDialog.h wMenu.h pObject.h FallingDemo.cpp pObject.cpp stdafx.cpp wWidget.cpp wContener.cpp wLabel.cpp wImage.cpp wDialog.cpp wMenu.cpp UserInterface.cpp
