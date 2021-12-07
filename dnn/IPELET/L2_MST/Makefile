CC = g++

TARGET = graph.dll
OBJECTS = graph.o ipeIO.o
IPE_PATH = C:\Users\Chaeyoon\ipe-7.2.23-win64\ipe-7.2.23\bin

all : $(TARGET)

%.o : %.cpp
	$(CC) -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude $< -o $@

$(TARGET) : $(OBJECTS)
	$(CC) -shared -o $(TARGET) $(OBJECTS) -L $(IPE_PATH) -lipe 

clean :
	del $(OBJECTS) $(TARGET)