CC = g++

TARGET = L2_seg_voronoi.dll
OBJECTS = Edge.o Segment.o Point.o Polygon.o PolygonClipping.o ..\ipeIO.o SVD_Function.o ipeSVD.o


all : $(TARGET)

%.o : %.cpp
	$(CC) -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -shared -o $(TARGET) $(OBJECTS) -L../bin -lipe -lipe

clean :
	del $(OBJECTS) $(TARGET)