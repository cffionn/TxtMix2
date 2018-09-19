CXX = g++
CXXFLAGS = -Wall -Werror -O2 -Wextra -Wno-unused-local-typedefs -Wno-deprecated-declarations -std=c++11 -g
ifeq "$(GCCVERSION)" "1"
  CXXFLAGS += -Wno-error=misleading-indentation
endif

INCLUDE=-I $(PWD)
ROOT=`root-config --cflags --glibs`

MKDIR_BIN=mkdir -p $(PWD)/bin
MKDIR_OUTPUT=mkdir -p $(PWD)/output

all: mkdirBin mkdirOutput bin/runTxtMix.exe bin/simpleConstructedPoem.exe

mkdirBin:
	$(MKDIR_BIN)

mkdirOutput:
	$(MKDIR_OUTPUT)

bin/runTxtMix.exe: src/runTxtMix.C
	$(CXX) $(CXXFLAGS) src/runTxtMix.C $(INCLUDE) $(ROOT) -o bin/runTxtMix.exe 

bin/simpleConstructedPoem.exe: src/simpleConstructedPoem.C
	$(CXX) $(CXXFLAGS) src/simpleConstructedPoem.C  $(INCLUDE) $(ROOT) -o bin/simpleConstructedPoem.exe

clean:
	rm -f ./*~
	rm -f src/*~
	rm -f include/*~
	rm -f configs/*~
	rm -f ./#*#
	rm -f src/#*#
	rm -f include/#*#
	rm -f configs/#*#
	rm -f bin/*.exe
	rm -rf bin
