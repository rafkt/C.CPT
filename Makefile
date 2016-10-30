CC=g++ -std=c++11

FLAGS=-O3 -g -DNDEBUG
FLAGS_SDSL_I=-I ~/include
FLAGS_SDSL_L=-L ~/lib
OFLAGS=-c
LIBS=-lsdsl -ldivsufsort -ldivsufsort64

EXEFLAGS=-o ./program.out

	REM=rm -f
	FND_FILES=find . -type f -name
	REM_FILES=-exec rm {} \;
	XF=.exe
	IN=.o

BUILD_PATH = Build
INCLUDE_PATH  = include
SEQUENCE_PATH = src/SequenceDatabase
INVERTEDINDEX_PATH = src/Interfaces/II
CCPT_path = src/Predictor/CCPT/

test: $(BUILD_PATH)/InvertedIndex.o $(BUILD_PATH)/sequence.o $(BUILD_PATH)/II_bit_vector.o
	$(CC) $(FLAGS) $(BUILD_PATH)/InvertedIndex.o $(BUILD_PATH)/sequence.o $(BUILD_PATH)/II_bit_vector.o $(EXEFLAGS)
$(BUILD_PATH)/sequence.o: $(SEQUENCE_PATH)/Sequence.cpp $(INCLUDE_PATH)/Sequence.h
	$(CC) $(OFLAGS) $(FLAGS) $(SEQUENCE_PATH)/Sequence.cpp -o $(BUILD_PATH)/sequence.o
$(BUILD_PATH)/InvertedIndex.o: $(INVERTEDINDEX_PATH)/InvertedIndex.cpp $(INCLUDE_PATH)/Sequence.h $(INCLUDE_PATH)/InvertedIndex.h
	$(CC) $(OFLAGS) $(FLAGS) $(INVERTEDINDEX_PATH)/InvertedIndex.cpp -o $(BUILD_PATH)/InvertedIndex.o
$(BUILD_PATH)/II_bit_vector.o: $(CCPT_path)/II_bit_vector.cpp $(INCLUDE_PATH)/InvertedIndex.h $(INCLUDE_PATH)/II_bit_vector.h
	$(CC) $(OFLAGS) $(FLAGS) $(CCPT_path)/II_bit_vector.cpp -o $(BUILD_PATH)/II_bit_vector.o

clean:
	$(REM) $(BUILD_PATH)/*$(XF)
	$(REM) $(BUILD_PATH)/*.out
	$(REM) *.out
	$(REM) $(BUILD_PATH)/program
	$(REM) $(BUILD_PATH)/*$(IN)
	$(FND_FILES) $(BUILD_PATH)/trainingFile\* $(REM_FILES)