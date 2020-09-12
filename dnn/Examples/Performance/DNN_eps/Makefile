CC = g++

TARGET = ..\DNN_eps.exe
OBJECTS = DNN_eps.o ..\..\..\NearestNeighbor\EpsGraph\eps_graph.o ..\..\..\NearestNeighbor\EpsGraph\point.o ..\..\..\NearestNeighbor\EpsGraph\polygon.o

all : $(TARGET)

%.o : %.cpp
	$(CC) -c $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean :
	del $(OBJECTS) $(TARGET)