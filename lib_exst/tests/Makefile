SHELL = /bin/sh
CXX = g++
CXXFLAGS = -std=c++11 -W -Wall
INCLUDE = -I../ -I.
ALL_CXXFLAGS = $(INCLUDE) $(CXXFLAGS)
TARGET = clasp_test
CPPUNIT_INCLUDE:=~/cppunit-1.12.1/include
CPPUNIT_LIB:=/usr/local/lib/libcppunit.so
exst_lib:=../lib/exst.a
sources:=$(wildcard *.cpp)
objects:=$(patsubst %.cpp,%.o,$(sources))
deps:=$(patsubst %.cpp,%.dep,$(sources))

$(TARGET):	$(exst_lib) $(objects) $(deps)
	$(CXX) $(ALL_CXXFLAGS) $(objects) $(exst_lib) $(CPPUNIT_LIB) -o $(TARGET)

ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),distclean)
    -include $(deps)
  endif
endif

%.o:	%.cpp
	$(CXX) $(ALL_CXXFLAGS) -I $(CPPUNIT_INCLUDE) -c $< -o $@

%.dep: %.cpp
	$(CXX) $(INCLUDE) -std=c++11 -W -Wall  -I $(CPPUNIT_INCLUDE) -MM $< > $@

$(exst_lib):
	$(MAKE)  -C ../src

.PHONY:	clean distclean
clean:
	@$(RM) $(TARGET)
	@$(RM) $(objects)
	@$(RM) $(deps)

distclean: clean


