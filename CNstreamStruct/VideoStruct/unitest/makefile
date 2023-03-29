CXX := g++
CXX_FLAGS := -ggdb3 -DLINUX -Wno-sign-compare -fPIC -std=c++0x

CORE_INC_PATH=../framework/include
CUDA_INC_PATH=/usr/local/cuda

INC := -I. -I$(CORE_INC_PATH) -I$(CUDA_INC_PATH)/include 
LIB := -L/usr/local/lib -L$(CUDA_INC_PATH)/lib64

SRC := $(shell find ./ -name "*.cpp")
SRC += $(shell find ../framework/source -name "*.cpp")

NVCC := /usr/local/cuda/bin/NVCC
NVCC_FLAGS := -g -G

NVCC_OBJ := $(NVCC_SOURCE:%.cu=%.o)
OBJ := $(SRC:%.cpp=%.o)
OBJ += $(NVCC_OBJ)

TARGET := ./xxx

.PHONY:all
all:$(TARGET) $(OBJ)
%.o:%.cpp
	$(CXX) $(CXX_FLAGS) $(INC) -o $@ -c $<
%.o:%.cu
	$(NVCC) $(NVCC_FLAGS) $(INC) -o $@ -c $<
$(TARGET):$(OBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJ) $(LIB)


.PHONY:clean
clean:
	rm -rf $(TARGET) $(OBJ) 

.PHONY:love
love:clean all











