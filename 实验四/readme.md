实验四生成目标代码

————————————————————————————————

main.cpp实现了将中间代码转换为目标代码，使用g++编译生成main可执行程序，将middle.txt文件输入到main程序，即可在对应目录文件下生成demo.asm文件，最终生成的目标代码为mips代码。

test.c为测试文件的源c程序

middle.txt为生成的中间代码文件

demo.asm为生成的最终代码文件



可以在Windows环境下使用QTSpim来运行demo.asm文件来查看寄存器的分配状态