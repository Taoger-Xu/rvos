-include common.mk

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \
	uart.c
OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

# 默认生成目标
.DEFAULT_GOAL := all

all: os.elf

# start.o must be the first in dependency!
# $< —— 依赖对象集中的第一个
# $^—— 所有的依赖对象集
# $@ —— 目标集
os.elf: $(OBJS)
	$(CC) $(CFLAGS) -Ttext=0x80000000 -o os.elf $^
	$(OBJCOPY) -O binary os.elf os.bin
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<

qemu: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

# 将生成的elf文件格式
.PHONY : show
show: all
	@${OBJDUMP} -S os.elf > rvos.txt

# 清除生成的目标文件
.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
