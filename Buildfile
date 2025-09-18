CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -mwindows -I. -Iheaders -Iheaders/crypto -Iheaders/file -Iheaders/tasks


MAIN_TARGET = main
CRYPTION_TARGET = cryption

MAIN_SRC = main.cpp \
           gui.cpp \
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
	$(CXX) $(CXXFLAGS) $^ -o $@ -lshell32 -lole32


$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(MAIN_OBJ) $(CRYPTION_OBJ) $(MAIN_TARGET) $(CRYPTION_TARGET)
clean:
	del /Q $(MAIN_OBJ) $(CRYPTION_OBJ) $(MAIN_TARGET).exe $(CRYPTION_TARGET).exe 2>nul || exit 0


.PHONY: clean all
