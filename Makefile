# 项目目录结构
# root dir
# ├── bin
# │   └── prod 可执行文件
# ├── include
# │   └── Daemonize.h
# ├── Makefile
# ├── obj
# │   ├── Daemonize.o
# │   └── main.o
# └── src
#     ├── Daemonize.cpp
#     └── main.cpp

# Which compiler
CC = g++

# use c++ 11 to compiler
C11 = -std=c++11

# Options for debug
# -g表示可以使用gdb调试
# -Wall是所有错误和警告在编译的时候都打出来
# -ggdb表示将尽可能的生成gdb的可以使用的调试信息.
CPPFLAGS = -g -ggdb -Wall -O0

# -O2: 包含-O1的优化并增加了不需要在目标文件大小和执行速度上进行折衷的优化.
#      编译器不执行循环展开以及函数内联.此选项将增加编译时间和目标文件的执行性能.
# Options for release
# CPPFLAGS = -Wall -O3

# 当前Makefile路径，工作目录绝对路径
# WORK_DIR = $(shell pwd)
# 当前Makefile路径，工作目录相对路径
WORK_DIR = .
# join：连接字符串，避免直接拼接时$(WORK_DIR)/src，产生莫名其妙的空格
# 头文件路径
INC_PATH = $(join $(WORK_DIR),/include)
# 源文件路径
SRC_PATH = $(join $(WORK_DIR),/src)
# object文件路径，放生成的.o文件
OBJ_PATH = $(join $(WORK_DIR),/obj)
# bin文件路径，放编译生成的可执行文件
BIN_PATH = $(join $(WORK_DIR),/bin)

# 自动获取所有.cpp文件，并将目标定义为同名.o文件
# wildcard:扩展通配符,例如将*进行匹配,可获取指定后缀名的文件，且带路径,$(wildcard *.c ./sub/*.c)
# notdir:去除路径，类似于basename的用法,$(notdir $(src))
# patsubst: 替换通配符,$(patsubst %.c，%.o，$(dir) )
SRC_FILE_WITH_DIR = $(wildcard $(join $(SRC_PATH),/*.cpp))
SRC_FILE = $(notdir $(SRC_FILE_WITH_DIR))
# 把所有.cpp替换为.o文件，两种方法
# OBJ_FILE = $(SRC_FILE:%.cpp=%.o)
# OBJ_FILE = $(patsubst %.cpp,%.o,$(SRC_FILE))
OBJ_FILE = $(SRC_FILE:%.cpp=%.o)

# 所有.o文件添加绝对路径
OBJS = $(patsubst %.o, $(OBJ_PATH)/%.o,$(OBJ_FILE))


# 编译生成的可执行文件名,带绝对路径
TARGET = $(join $(BIN_PATH),/myvector)

all: $(TARGET)

# Rule: compile .cpp to .o,指定了生成路径
# $@ 当前目标名
# $< 当前依赖的第一个文件的名字
$(OBJ_PATH)/%.o:$(SRC_PATH)/%.cpp
	$(CC) $(CPPFLAGS) ${C11} -I$(INC_PATH) -c $< -o $@

# $^ 所有的依赖文件
# -lpthread 必须放在.o文件或源文件后
$(TARGET): ${OBJS}
	$(CC) $(CPPFLAGS) ${C11} -o $@ $^

# 依赖关系，可以不写，不确定是否应写绝对路径
main.o:  main.cpp Vector.h
Vector.o: Vector.h

# 伪目标，没有依赖只有执行动作的目标
.PHONY: clean
clean:
	@rm $(OBJS) # @表示不在终端打印执行的命令
	@rm $(TARGET)

# g++ -std=c++11 -g -ggdb -Wall -O0 -I./include -c ./src/Vector.cpp -o ./obj/Vector.o
stalib:
	$(CC) $(CPPFLAGS) ${C11} -I$(INC_PATH) -c ./src/Vector.cpp -o ./obj/VectorSta.o
	ar -crv ./lib/libmyvector.a ./obj/VectorSta.o

dynlib:
	$(CC) $(CPPFLAGS) ${C11} -fPIC -I$(INC_PATH) -c ./src/Vector.cpp -o ./obj/VectorDyn.o
	$(CC) $(CPPFLAGS) ${C11} -shared -o ./lib/libmyvector.so ./obj/VectorDyn.o

usesta:
	$(CC) $(CPPFLAGS) ${C11} -I$(INC_PATH) -o ./bin/vec_usesta ./src/main.cpp -L./lib -lmyvector 

usedyn:
	$(CC) $(CPPFLAGS) ${C11} -I$(INC_PATH) -o ./bin/vec_usedyn ./src/main.cpp -L./lib -lmyvector 
# 测试用
path:
	@echo $(SRC_FILE_WITH_DIR)
