/*QEMU RISC-V Virt machine with 16550a UART and VirtIO MMIO*/
/* 定义RicsV芯片相关的物理参数以及特性 */
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*
 * maximum number of CPUs
 * CPU的最大数量
 */
#define MAXNUM_CPU 8

/*
 * MemoryMap,内存映射,IO与DRAM统一编址
 * 0x00001000 -- boot ROM, provided by qemu
 * 0x02000000 -- CLINT
 * 0x0C000000 -- PLIC
 * 0x10000000 -- UART0
 * 0x10001000 -- virtio disk
 * 0x80000000 -- boot ROM jumps here in machine mode, where we load our kernel
 */
/* This machine puts UART registers here in physical memory. */
#define UART0 0x10000000L
#endif 