CC = g++

TARGET = L1_voronoi.dll
OBJECTS = Edge.o L1voronoi.o Point.o Polygon.o PolygonClipping.o ..\ipeIO.o ipe_L1_voronoi.o


all : $(TARGET)

%.o : %.cpp
	$(CC) -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -shared -o $(TARGET) $(OBJECTS) -LC:/ipe-7.2.17/bin -lipe -lipe

clean :
	del $(OBJECTS) $(TARGET)