CXX      = g++
CXXFLAGS = -Wall -std=c++11 -g
TARGET	 = demo.exe
SRC		   = *.cpp
LIBS     = -L/usr/local/lib/ -lhiredis


$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o $(TARGET)