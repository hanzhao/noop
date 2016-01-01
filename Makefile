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


noop: $(ALL_OBJS)
	$(LINK) $(ALL_OBJS) -o noop

$(SRC_PATH)/%.o: $(SRC_PATH)/%.cc $(ALL_DEPS)
	$(CXX) $(CXXFLAGS) $(ALL_INCS) -c $< -o $@

clean_o:
	rm -f $(SRC_PATH)/*.o

clean_noop:
	rm -f noop

clean: clean_o clean_noop
