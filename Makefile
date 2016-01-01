CXX = clang++
CXXFLAGS = -O -W -Wall -g --std=c++11
LINK = $(CXX)

ALL_INCS = -I src

SRC_PATH = ./src

ALL_DEPS = $(SRC_PATH)/noop.h \
					 $(SRC_PATH)/noop_core.h \
					 $(SRC_PATH)/noop_io.h \
					 $(SRC_PATH)/noop_switches.h \
					 $(SRC_PATH)/noop_parser.h \
					 $(SRC_PATH)/noop_type.h

ALL_OBJS = $(SRC_PATH)/noop.o \
					 $(SRC_PATH)/noop_core.o \
					 $(SRC_PATH)/noop_io.o \
					 $(SRC_PATH)/noop_switches.o \
					 $(SRC_PATH)/noop_parser.o \
					 $(SRC_PATH)/noop_type.o


noop: noop.o noop_core.o noop_io.o noop_parser.o noop_switches.o noop_type.o
	$(LINK) $(ALL_OBJS) -o noop

noop.o: $(SRC_PATH)/noop.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $(SRC_PATH)/$@

noop_core.o: $(SRC_PATH)/noop_core.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $(SRC_PATH)/$@

noop_io.o: $(SRC_PATH)/noop_io.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $(SRC_PATH)/$@

noop_switches.o: $(SRC_PATH)/noop_switches.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $(SRC_PATH)/$@

noop_parser.o: $(SRC_PATH)/noop_parser.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $(SRC_PATH)/$@

noop_type.o: $(SRC_PATH)/noop_type.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $(SRC_PATH)/$@
