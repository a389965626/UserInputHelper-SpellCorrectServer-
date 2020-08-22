SRCS1:= $(wildcard ./Configuration/*.cc)
SRCS2:= $(wildcard ./TcpServer/*.cc)
SRCS3:= $(wildcard ./ThreadPool/*.cc)
SRCS4:= $(wildcard ./SpellCorrectServer/*.cc)
SRCS5:= $(wildcard ./Cache/*.cc)

OBJS1:= $(patsubst %.cc, %.o, $(SRCS1))
OBJS2:= $(patsubst %.cc, %.o, $(SRCS2))
OBJS3:= $(patsubst %.cc, %.o, $(SRCS3))
OBJS4:= $(patsubst %.cc, %.o, $(SRCS4))
OBJS5:= $(patsubst %.cc, %.o, $(SRCS5))

LIBS:= -lpthread -ljsoncpp -fPIC
CXX:=g++

CXXFLAGS:= -std=c++11  $(LIBS) 

EXE:=./inline_server.exe

$(EXE):$(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5)
	$(CXX) -o $(EXE) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4)  $(OBJS5) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5)

