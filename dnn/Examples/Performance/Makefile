CC = g++

TARGET = performance.exe
OBJECTS = main.o ..\..\NearestNeighbor\EpsGraph\eps_graph.o ..\..\NearestNeighbor\EpsGraph\point.o ..\..\NearestNeighbor\EpsGraph\polygon.o

all : $(TARGET)

%.o : %.cpp
	$(CC) -c $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	del $(OBJECTS) $(TARGET)