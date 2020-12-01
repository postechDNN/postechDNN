CC = g++

TARGET = ../L1_voronoi.so
OBJECTS = ../Edge.o ../L1voronoi.o ../Point.o ../Polygon.o ../PolygonClipping.o ../../IpeIO/ipeIO.o ../ipe_L1_voronoi.o
IPE = /Applications/Ipe.app

all : $(TARGET)

%.o : %.cpp
	$(CC) -std=c++11 -c -O2 -DWIN32 -fno-exceptions -fno-rtti -I$(IPE)/Contents/Frameworks/Headers $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -std=c++11 -bundle -o $(TARGET) $(OBJECTS) -L$(IPE)/Contents/Frameworks/ -lipe

clean :
	rm $(OBJECTS) $(TARGET)
