BIN=bin
BUILD=build
SRC=src
PROJ =msp
OBJS = msp.o
SOURCE=$(wildcard $(SRC)/MSP_ser.cpp)
CXX = g++
RM = rm -rf 
CFLAGS = -c 
LIBS =boost_system
$(BIN)/$(PROJ):$(BUILD)/$(OBJS)
		@mkdir -p bin
		$(CXX) $^  -l$(LIBS) -std=c++11 -o $@
$(BUILD)/$(OBJS):$(SOURCE)
		@mkdir -p build
		$(CXX)  $(CFLAGS) $^ -std=c++11 -l$(LIBS) -o $@
clean:
		$(RM) $(BIN)/* $(BUILD)/* 
