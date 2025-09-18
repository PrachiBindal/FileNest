CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I. -Iheaders -Iheaders/crypto -Iheaders/file -Iheaders/tasksmingw32-make -f Buildfile

MAIN_TARGET = main
CRYPTION_TARGET = cryption

MAIN_SRC = main.cpp \
           src/tasks/ProcessManagement.cpp \
           src/file/IO.cpp \
           src/file/ReadEnv.cpp \
           src/crypto/Cryption.cpp

CRYPTION_SRC = src/crypto/CryptionMain.cpp \
               src/crypto/Cryption.cpp \
               src/file/IO.cpp \
               src/file/ReadEnv.cpp

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
CRYPTION_OBJ = $(CRYPTION_SRC:.cpp=.o)

all: $(MAIN_TARGET) $(CRYPTION_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN_OBJ) $(CRYPTION_OBJ) $(MAIN_TARGET) $(CRYPTION_TARGET)

.PHONY: clean all