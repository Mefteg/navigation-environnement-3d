SOURCES =	princ.cpp vertex.cpp face.cpp forme.cpp
OBJECTS	=	$(SOURCES:.cpp=.o)
TARGET	=	princ
LIBS	=	$(shell sdl-config --libs) -lGL -lGLU 

all: $(OBJECTS)
	g++ $(LIBS) -o $(TARGET) $(OBJECTS)

%o: %.cpp %.h
	g++ -o $@ -c $<

x: all
	./$(TARGET)

clean:
	rm -rf $(OBJECTS)

superclean : clean
	rm -rf $(TARGET)

