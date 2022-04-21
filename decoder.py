import re
import os
import sys
from prettytable import PrettyTable


def pretty_print_instruction(ins: str) -> None:
    ins_dict, header = riscv_instruction_parser(ins)
    print("bit-level representation:\n\t"+ins_dict["bin"])
    if not header:
        return
    table = PrettyTable()
    for h in header:
        table.add_column(h, [ins_dict[h]])
    print(table)
    print()

def riscv_instruction_parser(ins: str):
    result = {}
    if len(ins) == 8:
        ins_bin = format(int(ins, 16), '032b')
        result.update({"bin": ins_bin})
        opcode = ins_bin[-7:]
        rd = ins_bin[-12:-7]
        funct3 = ins_bin[-15:-12]
        rs1 = ins_bin[-20:-15]
        rs2 = ins_bin[-25:-20]
        funct7 = ins_bin[-32:-25]
        if opcode in ["0000011", "0001111", "0010011","0011011", "1100111"]:  # I
            imm = funct7+rs2
            result.update({
                "imm[11:0]": imm,
                "rs1": rs1,
                "funct3": funct3,
                "rd": rd,
                "opcode": opcode
            })
            header = ["imm[11:0]", "rs1", "funct3", "rd", "opcode"]
        elif opcode in ["0010111", "0110111"]:  # U
            result.update({
                "imm[31:12]": funct7+rs2+rs1+funct3,
                "rd": rd,
                "opcode": opcode
            })
            header = ["imm[31:12]", "rd", "opcode"]
        elif opcode == "0100011":  # S
            result.update({
                "imm[11:5]": funct7,
                "rs2": rs2,
                "rs1": rs1,
                "funct3": funct3,
                "imm[4:0]": rd,
                "opcode": opcode
            })
            header = ["imm[11:5]", "rs2", "rs1",
                      "funct3", "imm[4:0]", "opcode"]
        elif opcode in ["0110011", "0111011"]:  # R
            result.update({
                "funct7": funct7,
                "rs2": rs2,
                "rs1": rs1,
                "funct3": funct3,
                "rd": rd,
                "opcode": opcode
            })
            header = ["funct7", "rs2", "rs1", "funct3", "rd", "opcode"]
        elif opcode == "1100011":  # SB
            result.update({
                "imm[12|10:5]": funct7,
                "rs2": rs2,
                "rs1": rs1,
                "funct3": funct3,
                "imm[4:1|11]": rd,
                "opcode": opcode
            })
            header = ["imm[12|10:5]", "rs2", "rs1",
                      "funct3", "imm[4:1|11]", "opcode"]
        elif opcode == "1101111":  # UJ
            result.update({
                "imm[20|10:1|11|19:12]": funct7+rs2+rs1+funct3,
                "rd": rd,
                "opcode": opcode
            })
            header = ["imm[20|10:1|11|19:12]", "rd", "opcode"]
        else:
            header = []
    elif len(ins) == 4:
        ins_bin = format(int(ins, 16), '016b')
        result.update({"bin": ins_bin})
        op = ins_bin[-2:]
        rs2 = ins_bin[-7:-2]
        rd_rs1 = ins_bin[-12:-7]
        funct4 = ins_bin[-16:-12]
        header = []
    # print(ins_bin)

    return result, header


def main():
    rv_ins_flag = re.compile("[0-9a-fA-F]:\s[0-9a-fA-F]{4}")
    rv_32bit_ins = re.compile("[0-9a-fA-F]{8}")
    rv_16bit_ins = re.compile("[0-9a-fA-F]{4}")
    if len(sys.argv) == 1:
        print("Missing option [filename] for decoding!")
        print("""usage: python decoder.py filename\n\nOption 'filename' should be relative path to the file for decoding.
And it should be disassembled code by objdump.""")
    else:
        filename = sys.argv[1]
        with open(file=os.path.join(os.getcwd(), filename), encoding="utf-8") as f:
            for line in f:
                print(line, end="")
                if rv_ins_flag.search(line) is None:
                    continue
                riscv_ins = rv_32bit_ins.search(line)
                if riscv_ins is not None:
                    ins = line[riscv_ins.start(): riscv_ins.end()]
                    pretty_print_instruction(ins)

                else:
                    continue
                    # 暂不支持压缩指令
                    riscv_ins = rv_16bit_ins.search(line)
                    ins = line[riscv_ins.start(): riscv_ins.end()]
                    pretty_print_instruction(ins)
if __name__ == '__main__':
    main()