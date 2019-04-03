PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC=gcc
CXX=g++

INCLUDE  += \
	-I$(PROJECT_ROOT)/include

BUILDDIR = build
BUILDDIR_APP = $(BUILDDIR)/app
BUILDDIR_LIB = $(BUILDDIR)/lib
OUTPUT_DIR = output
BUILDDIR_LIB_OBJS = \
	$(BUILDDIR_LIB)/CRTCBoundData.o \
	$(BUILDDIR_LIB)/CCensusFileManager.o
	
CFLAGS += -g -lpq

CXXFLAGS += -DDEBUG -g

LIBS += \
	$(OUTPUT_DIR)/lib_census_file_manager.a

$(BUILDDIR_APP)/%.o:	$(PROJECT_ROOT)/src/app/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) --std=c++11 $(CPPFLAGS)  $(INCLUDE) -o $@ $<

$(BUILDDIR_LIB)/%.o:	$(PROJECT_ROOT)/src/lib/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) --std=c++11 $(CPPFLAGS)  $(INCLUDE) -o $@ $<
	
all: tree libs

tree:
	@mkdir -p $(BUILDDIR_APP)
	@mkdir -p $(BUILDDIR_LIB)
	@mkdir -p $(OUTPUT_DIR)
libs: $(LIBS)

$(OUTPUT_DIR)/lib_census_file_manager.a : $(BUILDDIR_LIB_OBJS)
	$(AR) rcs $@ $(BUILDDIR_LIB_OBJS)
	
clean:
	rm -fr $(BUILDDIR) $(OUTPUT_DIR)
