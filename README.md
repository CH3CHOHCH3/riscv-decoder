# RISCV 机器码指令拆分器

众所周知，RISCV 的机器码字段布局有很多种，如果手动根据 opcode 分析各个字段的值是一件累人的工作。

这个由 Qt6.2 开发的软件以及如下python脚本，能帮助我们把一条 32 位的十六进制机器码根据 opcode 合理地拆分各个字段。

<div align="center">
  <img src="https://user-images.githubusercontent.com/72899584/163538153-204c97ff-7069-441d-8445-d0bbff852c14.png" />
  <p>Qt软件使用截图</p>
</div>

python脚本需从命令行调用，因使用了`prettytable`库以获得更规整的视觉效果，需要首先用pip安装：

````shell
pip install prettytable
````

然后指定python解释器调用该脚本，参数为对`.o`文件使用`objdump -D`命令生成的反汇编文件名，要求使用相对路径（因脚本中会拼接为绝对路径），同一目录下键入文件名即可。命令中`my_objdump.s`为示例的反汇编文件名。

```shell
python3 decoder.py my_objdump.s
```

该脚本使用输出的示例如下：

```assembly
Disassembly of section .text:

0000000000000000 <multiply>:
   0:	00050793          	mv	a5,a0
bit-level representation:
	00000000000001010000011110010011
+--------------+-------+--------+-------+---------+
|  imm[11:0]   |  rs1  | funct3 |   rd  |  opcode |
+--------------+-------+--------+-------+---------+
| 000000000000 | 01010 |  000   | 01111 | 0010011 |
+--------------+-------+--------+-------+---------+
...
```

可以在上述命令后添加重定向命令以直接获得完整的文本，如将命令更改为：

```shell
python3 decoder.py my_objdump.s > my_decoder.s
```

此时`my_decoder.s`文件中的文本，即如上述示例，已经插入了机器指令的二进制表示，以及各字段分解的表格。

---

目前已经支持的指令包括 `RV32I Base Instruction Set`(除去 `FENCE`，`ECALL`，`EBREAK`)，`RV64I Base Instruction Set`和各自的standard extension（可在<a href="https://five-embeddev.com/riscv-isa-manual/latest/instr-table.html">此表</a>中对照），暂不支持 16 位压缩指令。

riscv64-linux-gnu-gcc 的 `-c` 指令会默认生成带压缩指令的机器码，可以使用如下编译指令使产生的`test.o`目标模块文件中`.text`代码段中只含32位指令：

```
riscv64-linux-gnu-gcc -c -march=rv64g test.s
```

## 注意

+ release 中打包了经过编译、开箱即用的软件，如果没有 Qt 环境可以自取。
+ 对于`j`或是`li`这样的伪指令，本软件会按照其机器码对应的原指令直接拆分（即`jal`和`addi`）
+ 本软件并不能由机器码指令输出对应的汇编指令，这样设计是考虑到 `objdump` 已经完成了这一工作，因此，本软件应配合 `objdump` 使用。
+ 软件中 RV32I 和 RV64I 的主要区别在于，在 RV32I 中，`SLLI`，`SRLI`，`SRAI` 的 `shamt` 字段占 5 位，而在 RV64I 中占 6 位。具体数据来自于上面提到的表。
+ 如果您发现了错误，请在 Issues 中告知我，十分感谢；也欢迎您提交 pr 来丰富本软件的功能，或使其支持其他指令集。
