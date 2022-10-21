#include "platform.h"
    # size of each hart's stack is 1024 bytes
    .equ STACK_SIZE, 1024
    .global _start # Define entry _start
    .text # Define beginning of text section

_start:
    # 读取当前hartid,如果hart_id==0正常运行,否则跳转到park
    csrr t0, mhartid    # read current hart id
    mv tp, t0   # keep CPU's hartid in its tp for later usage
    bnez t0, park   # if we are not on the hart 0, we jump to the park

    # 准备堆栈，为C语言的函数调用准备环境
    # stack grows from bottom to top, stack pointer即栈顶指针指向低地址
    slli	t0, t0, 10		# shift left the hart id by 1024
    la sp, stacks + STACK_SIZE # 初始化stack pointer指向the end of the first stack space
    add sp, sp, t0 # 根据hart_id的不同让不同的hart的sp指向对应的初始化地址,比如hart_1指向stacks+1024*2

    # jump到c函数执行
    j start_kernel		# hart 0 jump to c


					
park:
    wfi # cpu低功耗休眠
    j park # 让CPU处于空转状态

    # 分配堆栈
stacks:
    .skip  STACK_SIZE * MAXNUM_CPU # 为所有的harts分配对应的stacks空间
    .end # End of file


