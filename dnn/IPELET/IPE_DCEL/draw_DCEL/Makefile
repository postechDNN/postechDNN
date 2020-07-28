CC = g++

TARGET = draw_DCEL.dll
OBJECTS = ..\..\IpeIO\ipeIO.o ipe_draw_dcel.o ..\..\..\DataStrcutures\DCEL\DCEL.o ..\..\..\DataStrcutures\DCEL\Edge.o ..\..\..\DataStrcutures\DCEL\Point.o


all : $(TARGET)

%.o : %.cpp
	$(CC) -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -shared -o $(TARGET) $(OBJECTS) -LC:/ipe-7.2.17/bin -lipe

clean :
	del $(OBJECTS) $(TARGET)