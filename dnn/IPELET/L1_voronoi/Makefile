CC = g++

TARGET = L1_voronoi.dll
OBJECTS = Edge.o L1voronoi.o Point.o Polygon.o PolygonClipping.o ..\IpeIO\ipeIO.o ipe_L1_voronoi.o
IPE=C:/ipe-7.2.17

all : $(TARGET)

%.o : %.cpp
	$(CC) -std=c++11 -c -O2 -DWIN32 -fno-exceptions -fno-rtti -I$(IPE)/include $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -std=c++11 -shared -o $(TARGET) $(OBJECTS) -L$(IPE)/bin -lipe

clean :
	del $(OBJECTS) $(TARGET)