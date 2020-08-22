SRCS:= $(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))

CXX:=g++

CXXFLAGS:= -std=c++11 -I -fPIC

EXE:=./offline.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)

