ifndef TARGET_COMPILE
    $(error TARGET_COMPILE not set)
endif

ifndef KP_DIR
    KP_DIR = ../..
endif

CC = $(TARGET_COMPILE)gcc
LD = $(TARGET_COMPILE)ld

# 包含路径
INCLUDE_DIRS := . include patch/include linux/include linux/arch/arm64/include linux/tools/arch/arm64/include
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/kernel/$(dir))

# 编译选项
CFLAGS = -Wall -O2 -fno-stack-protector

# 模块对象文件
objs := kpm-termux-unmount.o

# 编译目标
all: kpm-termux-unmount.kpm

kpm-termux-unmount.kpm: ${objs}
	${CC} -r -o $@ $^

# 编译规则
%.o: %.c
	${CC} $(CFLAGS) $(INCLUDE_FLAGS) -c -O2 -o $@ $<

# 清理目标
.PHONY: clean
clean:
	rm -rf *.kpm
	find . -name "*.o" | xargs rm -f
