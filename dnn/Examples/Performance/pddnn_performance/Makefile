CC = g++

TARGET = pddnn_performance.exe
OBJECTS = pddnn_performance.o ..\..\..\NearestNeighbor\PDDNN\PDGraph.o ..\..\..\NearestNeighbor\PDDNN\PDNode.o \
..\..\..\DataStructures\DCEL\SimplePolygon.o ..\..\..\DataStructures\DCEL\Point.o ..\..\..\DataStructures\DCEL\Edge.o ..\..\..\DataStructures\DCEL\PolygonalDomain.o

all : $(TARGET)

%.o : %.cpp
	$(CC) -c $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	del $(OBJECTS) $(TARGET)