ARDUINO_HOME = C:/Program Files (x86)/Arduino
ARDUINO_LIB = $(ARDUINO_HOME)/libraries
CC = $(ARDUINO_HOME)/hardware/tools/avr/bin/avr-g++.exe

UNTIY_HOME = ../../unity
UNITY_SRC = $(UNTIY_HOME)/src

EXECUTABLE = refactor_calc
SRC_EXT = cpp
OBJ_EXT = o
MAIN = uno_ardu_sim.$(SRC_EXT)

COMMAND_DELETE = -rm -f
CC_FLAGS = -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions
CC_FLAGS += -ffunction-sections -fdata-sections -fno-threadsafe-statics
CC_FLAGS += -flto -w -x c++ -CC -mmcu=atmega328p -DF_CPU=16000000L
CC_FLAGS += -DARDUINO=10805 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR

CC_INCLUDE += -I"$(ARDUINO_HOME)/hardware/arduino/avr/cores/arduino"
CC_INCLUDE += -I"$(ARDUINO_HOME)/hardware/arduino/avr/variants/standard"
CC_INCLUDE += -I"$(ARDUINO_LIB)/LiquidCrystal/src"

.PHONY: clean_exec

#Remove executable -> build -> run
all: main

main:
	"$(CC)" $(CC_FLAGS) $(CC_INCLUDE) $(MAIN)
