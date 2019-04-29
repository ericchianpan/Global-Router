CXX = g++
CXXFLAGS = -O3 -std=c++11
SRC = src/main.cpp src/parser.cpp src/router.cpp src/routingdb.cpp src/tree.cpp \
		src/lib/mst.cpp src/lib/graphsp.cpp src/lib/BinaryHeap.cpp
OBJECTS = $(SRC:.cpp=.o)
EXECUTABLE = gr

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)

$(OBJDIS)/.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	rm -f src/*.o src/lib/*.o $(EXECUTABLE)

a1:
	(time ./gr input/adaptec1.capo70.2d.35.50.90.gr a1.out)
a2:
	(time ./gr input/adaptec2.mpl60.2d.35.20.100.gr a2.out)
a3:
	(time ./gr input/adaptec3.dragon70.2d.30.50.90.gr a3.out)
a4:
	(time ./gr input/adaptec4.aplace60.2d.30.50.90.gr a4.out)
a5:
	(time ./gr input/adaptec5.mfar50.2d.50.20.100.gr a5.out)
n1:
	(time ./gr input/newblue1.ntup50.2d.30.50.90.gr n1.out)
n2:
	(time ./gr input/newblue2.fastplace90.2d.50.20.100.gr n2.out)
n3:
	(time ./gr input/newblue3.kraftwerk80.2d.40.50.90.gr n3.out)
example:
	(time ./gr input/example.gr example.out)
