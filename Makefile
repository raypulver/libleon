CXX=g++
CXXFLAGS=-std=c++11 -funsigned-char -fPIC
OBJECTS=leon.o buffer-iterator.o type-check.o string-buffer.o encoder.o parser.o value.o exceptions.o
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -shared -o libleon.so

leon.o:
	$(CXX) $(CXXFLAGS) leon.cc -c

string-buffer.o:
	$(CXX) $(CXXFLAGS) string-buffer.cc -c

encoder.o:
	$(CXX) $(CXXFLAGS) encoder.cc -c

parser.o:
	$(CXX) $(CXXFLAGS) parser.cc -c

value.o:
	$(CXX) $(CXXFLAGS) value.cc -c

buffer-iterator.o:
	$(CXX) $(CXXFLAGS) buffer-iterator.cc -c

type-check.o:
	$(CXX) $(CXXFLAGS) type-check.cc -c

test.o:
	$(CXX) $(CXXFLAGS) test.cc -c

exceptions.o:
	$(CXX) $(CXXFLAGS) exceptions.cc -c

clean:
	rm -rf *.o *.so

install:
	cp libleon.so /usr/lib
	cp leon.h /usr/include
