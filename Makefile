CC=g++ -std=c++11

FLAGS=-O3 -g -DNDEBUG -DSD_CONSTRACTION
#-DSD_CONSTRACTION is used for adding an extra variable on PredictionTree node.
#If the flag is not defined then currently the SD_Tree cannot be compiled.
#So remove it from compile chain
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
INTERFACE_PATH = src/Interfaces
CCPT_path = src/Predictor/CCPT
PREDICTOR_path = src/Predictor
PROFILE_path = src/Profile
LIB_path = src/lib
EVALUATOR_path = src/Evaluator

#Assure that SDSL-Lite library is uptodate and compiled
#Include & Libary directories should be under Home folder

all: $(BUILD_PATH)/InvertedIndex.o $(BUILD_PATH)/sequence.o $(BUILD_PATH)/II_bit_vector.o $(BUILD_PATH)/PredictionTree.o $(BUILD_PATH)/CPT_Trie.o $(BUILD_PATH)/CPT_SD_Tree.o $(BUILD_PATH)/Predictor.o $(BUILD_PATH)/CPTPredictor.o $(BUILD_PATH)/Profile.o $(BUILD_PATH)/DatabaseHelper.o $(BUILD_PATH)/SD_CPTPredictor.o $(BUILD_PATH)/EF_II_bit_vector.o $(BUILD_PATH)/Elias_Fano.o $(BUILD_PATH)/Evaluator.o $(BUILD_PATH)/Encoder.o $(BUILD_PATH)/FIFRaw.o $(BUILD_PATH)/CPTPlusPredictor.o
	$(CC) $(FLAGS) $(FLAGS_SDSL_L) $(BUILD_PATH)/InvertedIndex.o $(BUILD_PATH)/sequence.o $(BUILD_PATH)/II_bit_vector.o $(BUILD_PATH)/CPT_Trie.o $(BUILD_PATH)/CPT_SD_Tree.o $(BUILD_PATH)/PredictionTree.o $(BUILD_PATH)/Predictor.o $(BUILD_PATH)/CPTPredictor.o $(BUILD_PATH)/SD_CPTPredictor.o $(BUILD_PATH)/EF_II_bit_vector.o $(BUILD_PATH)/Elias_Fano.o $(BUILD_PATH)/Profile.o $(BUILD_PATH)/DatabaseHelper.o $(BUILD_PATH)/Evaluator.o $(BUILD_PATH)/Encoder.o $(BUILD_PATH)/FIFRaw.o $(BUILD_PATH)/CPTPlusPredictor.o $(EXEFLAGS) $(LIBS)
$(BUILD_PATH)/sequence.o: $(SEQUENCE_PATH)/Sequence.cpp $(INCLUDE_PATH)/Sequence.h
	$(CC) $(OFLAGS) $(FLAGS) $(SEQUENCE_PATH)/Sequence.cpp -o $(BUILD_PATH)/sequence.o
$(BUILD_PATH)/InvertedIndex.o: $(INTERFACE_PATH)/II/InvertedIndex.cpp $(INCLUDE_PATH)/Sequence.h $(INCLUDE_PATH)/InvertedIndex.h
	$(CC) $(OFLAGS) $(FLAGS) $(INTERFACE_PATH)/II/InvertedIndex.cpp -o $(BUILD_PATH)/InvertedIndex.o
$(BUILD_PATH)/II_bit_vector.o: $(CCPT_path)/II_bit_vector.cpp $(INCLUDE_PATH)/InvertedIndex.h $(INCLUDE_PATH)/II_bit_vector.h
	$(CC) $(OFLAGS) $(FLAGS) $(CCPT_path)/II_bit_vector.cpp -o $(BUILD_PATH)/II_bit_vector.o
$(BUILD_PATH)/PredictionTree.o: $(INTERFACE_PATH)/Trie/PredictionTree.cpp $(INCLUDE_PATH)/PredictionTree.h
	$(CC) $(OFLAGS) $(FLAGS) $(INTERFACE_PATH)/Trie/PredictionTree.cpp -o $(BUILD_PATH)/PredictionTree.o
$(BUILD_PATH)/CPT_Trie.o: $(CCPT_path)/CPT_Trie.cpp $(INCLUDE_PATH)/CPT_Trie.h $(INCLUDE_PATH)/PredictionTree.h
	$(CC) $(OFLAGS) $(FLAGS) $(CCPT_path)/CPT_Trie.cpp -o $(BUILD_PATH)/CPT_Trie.o
$(BUILD_PATH)/CPT_SD_Tree.o: $(CCPT_path)/CPT_SD_Tree.cpp $(INCLUDE_PATH)/CPT_SD_Tree.h $(INCLUDE_PATH)/PredictionTree.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(CCPT_path)/CPT_SD_Tree.cpp -o $(BUILD_PATH)/CPT_SD_Tree.o
$(BUILD_PATH)/Predictor.o: $(PREDICTOR_path)/Predictor.cpp $(INCLUDE_PATH)/Predictor.h $(INCLUDE_PATH)/Sequence.h
	$(CC) $(OFLAGS) $(FLAGS) $(PREDICTOR_path)/Predictor.cpp -o $(BUILD_PATH)/Predictor.o
$(BUILD_PATH)/CPTPredictor.o: $(CCPT_path)/CPTPredictor.cpp $(INCLUDE_PATH)/CPTPredictor.h $(INCLUDE_PATH)/Sequence.h $(INCLUDE_PATH)/PredictionTree.h $(INCLUDE_PATH)/InvertedIndex.h $(INCLUDE_PATH)/Profile.h $(INCLUDE_PATH)/Predictor.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(CCPT_path)/CPTPredictor.cpp -o $(BUILD_PATH)/CPTPredictor.o
$(BUILD_PATH)/SD_CPTPredictor.o: $(CCPT_path)/SD_CPTPredictor.cpp $(INCLUDE_PATH)/CPTPredictor.h $(INCLUDE_PATH)/SD_CPTPredictor.h $(INCLUDE_PATH)/CPT_SD_Tree.h $(INCLUDE_PATH)/EF_II_bit_vector.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(CCPT_path)/SD_CPTPredictor.cpp -o $(BUILD_PATH)/SD_CPTPredictor.o
$(BUILD_PATH)/Profile.o: $(PROFILE_path)/Profile.cpp $(INCLUDE_PATH)/Profile.h
	$(CC) $(OFLAGS) $(FLAGS) $(PROFILE_path)/Profile.cpp -o $(BUILD_PATH)/Profile.o
$(BUILD_PATH)/DatabaseHelper.o: $(SEQUENCE_PATH)/DatabaseHelper.cpp $(INCLUDE_PATH)/DatabaseHelper.h $(INCLUDE_PATH)/Sequence.h $(INCLUDE_PATH)/Profile.h
	$(CC) $(OFLAGS) $(FLAGS) $(SEQUENCE_PATH)/DatabaseHelper.cpp -o $(BUILD_PATH)/DatabaseHelper.o
$(BUILD_PATH)/EF_II_bit_vector.o: $(INCLUDE_PATH)/II_bit_vector.h $(INCLUDE_PATH)/EF_II_bit_vector.h $(CCPT_path)/EF_II_bit_vector.cpp $(INCLUDE_PATH)/Elias_Fano.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(CCPT_path)/EF_II_bit_vector.cpp -o $(BUILD_PATH)/EF_II_bit_vector.o
$(BUILD_PATH)/Elias_Fano.o: $(INCLUDE_PATH)/Elias_Fano.h $(LIB_path)/Elias_Fano.cpp
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(LIB_path)/Elias_Fano.cpp -o $(BUILD_PATH)/Elias_Fano.o
$(BUILD_PATH)/Evaluator.o: $(INCLUDE_PATH)/Evaluator.h $(INCLUDE_PATH)/DatabaseHelper.h $(INCLUDE_PATH)/Predictor.h $(INCLUDE_PATH)/Sequence.h $(EVALUATOR_path)/Evaluator.cpp $(INCLUDE_PATH)/CPTPredictor.h $(INCLUDE_PATH)/SD_CPTPredictor.h $(INCLUDE_PATH)/CPTPlusPredictor.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(EVALUATOR_path)/Evaluator.cpp -o $(BUILD_PATH)/Evaluator.o
$(BUILD_PATH)/Encoder.o: $(INCLUDE_PATH)/Encoder.h $(INTERFACE_PATH)/Encoder/Encoder.cpp $(INCLUDE_PATH)/Sequence.h
	$(CC) $(OFLAGS) $(FLAGS) $(INTERFACE_PATH)/Encoder/Encoder.cpp -o $(BUILD_PATH)/Encoder.o
$(BUILD_PATH)/FIFRaw.o: $(INCLUDE_PATH)/FIFRaw.h $(LIB_path)/FIFRaw.cpp $(INCLUDE_PATH)/Sequence.h
	$(CC) $(OFLAGS) $(FLAGS) $(LIB_path)/FIFRaw.cpp -o $(BUILD_PATH)/FIFRaw.o
$(BUILD_PATH)/CPTPlusPredictor.o: $(INCLUDE_PATH)/CPTPredictor.h $(CCPT_path)/CPTPlusPredictor.cpp $(INCLUDE_PATH)/CPTPlusPredictor.h $(INCLUDE_PATH)/FIFRaw.h $(INCLUDE_PATH)/Encoder.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) $(CCPT_path)/CPTPlusPredictor.cpp -o $(BUILD_PATH)/CPTPlusPredictor.o

clean:
	$(REM) $(BUILD_PATH)/*$(XF)
	$(REM) $(BUILD_PATH)/*.out
	$(REM) *.out
	$(REM) $(BUILD_PATH)/program
	$(REM) $(BUILD_PATH)/*$(IN)
	$(FND_FILES) $(BUILD_PATH)/trainingFile\* $(REM_FILES)