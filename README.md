# RISCV 机器码指令拆分器

众所周知，RISCV 的机器码字段布局有很多种，如果手动根据 opcode 分析各个字段的值是一件累人的工作。

这个由 Qt6.2 开发的软件能帮助我们把一条 32 位的十六进制机器码根据 opcode 合理地拆分各个字段。

目前已经支持的指令包括 `RV32I Base Instruction Set`(除去 `FENCE`，`ECALL`，`EBREAK`)和 `RV64I Base Instruction Set`，暂不支持 16 位压缩指令。

riscv64-linux-gnu-gcc 的 `-c` 指令会默认生成带压缩指令的机器码，可以使用如下编译指令使产生的`test.o`目标模块文件中`.text`代码段中只含32位指令：

```
riscv64-linux-gnu-gcc -c -march=rv64g test.s
```

## 注意

1. 本软件并不能由机器码指令输出对应的汇编指令，这样设计是考虑到 objdump 已经完成了这一工作，因此，本软件应配合 objdump 使用。

2. 软件中 RV32I 和 RV64I 的主要区别在于，在 RV32I 中，`SLLI`，`SRLI`，`SRAI` 的 `shamt` 字段占 5 位，而在 RV64I 中占 6 位。具体数据可参阅<a href="https://five-embeddev.com/riscv-isa-manual/latest/instr-table.html">这里</a>

3. 如果您发现了错误，请在 Issues 中告知我，十分感谢；也欢迎您提交 pr 来丰富本软件的功能，或使其支持其他指令集。
