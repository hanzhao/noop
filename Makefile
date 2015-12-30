CXX = clang++
CXXFLAGS = -O -W -Wall -Werror -g --std=c++11
LINK = $(CXX)

ALL_INCS = -I src
ALL_DEPS = src/noop.h \
					 src/noop_core.h \
					 src/noop_io.h \
					 src/noop_switches.h \

noop: src/noop.o src/noop_core.o src/noop_io.o src/noop_switches.o
	$(LINK) -o noop \
					src/noop.o src/noop_core.o src/noop_io.o src/noop_switches.o

src/noop.o: $(ALL_DEPS) src/noop.cc
	$(CXX) -c $(CXXFLAGS) $(ALL_INCS) \
					-o src/noop.o src/noop.cc

src/noop_core.o: $(ALL_DEPS) src/noop_core.cc
	$(CXX) -c $(CXXFLAGS) $(ALL_INCS) \
					-o src/noop_core.o src/noop_core.cc

src/noop_io.o: $(ALL_DEPS) src/noop_io.cc
	$(CXX) -c $(CXXFLAGS) $(ALL_INCS) \
					-o src/noop_io.o src/noop_io.cc

src/noop_switches.o: $(ALL_DEPS) src/noop_switches.cc
	$(CXX) -c $(CXXFLAGS) $(ALL_INCS) \
					-o src/noop_switches.o src/noop_switches.cc
