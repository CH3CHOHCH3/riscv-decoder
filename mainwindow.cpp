#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::decode);
}

void MainWindow::decode(){
    bool valid = false;
    QString raw = ui->lineEdit->text();
    qint64 val = raw.toLongLong(&valid, 16);

    if(valid == false){
        QMessageBox::critical(
            this,
            "错误",
            "不是合法的十六进制数",
            QMessageBox::Ok,
            QMessageBox::Ok
        );
        return;
    }
    if(raw.length() == 8){
        handle32(val);
     }
    else{
        QMessageBox::critical(
            this,
            "错误",
            "不是预期的riscv指令",
            QMessageBox::Ok,
            QMessageBox::Ok
        );
    }
}

void MainWindow::handle32(qint64 val){
    QString ins = QString("%1").arg(val, 32, 2, QLatin1Char('0'));
    ui->textBrowser->setText(ins + "\n");
    QString opcode = ins.mid(25, -1);

    if(opcode == "0110011" or opcode == "0111011"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "funct7:" + ins.mid(0, 7) + "\n" +
            "funct3:" + ins.mid(17, 3) + "\n" +
            "rs1:" + ins.mid(12, 5) + "\n" +
            "rs2:" + ins.mid(7, 5) + "\n" +
            "rd:" + ins.mid(20, 5) + "\n" +
            "opcode:" + opcode
        );
    }
    else if(opcode == "1100011"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "funct3:" + ins.mid(17, 3) + "\n" +
            "rs1:" + ins.mid(12, 5) + "\n" +
            "rs2:" + ins.mid(7, 5) + "\n" +
            "imm:" + ins.mid(0, 1) + ins.mid(24,1) + ins.mid(1,6) + ins.mid(20,4) + "\n" +
            "opcode:" + opcode
        );
    }
    else if(opcode == "0000011"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "funct3:" + ins.mid(17, 3) + "\n" +
            "rs1:" + ins.mid(12, 5) + "\n" +
            "rd:" + ins.mid(20, 5) + "\n" +
            "imm:" + ins.mid(0, 12) + "\n" +
            "opcode:" + opcode
        );
    }
    else if(opcode == "0100011"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "funct3:" + ins.mid(17, 3) + "\n" +
            "rs1:" + ins.mid(12, 5) + "\n" +
            "rs2:" + ins.mid(7, 5) + "\n" +
            "imm:" + ins.mid(0, 7) + ins.mid(20, 5) + "\n" +
            "opcode:" + opcode
        );
    }
    else if(opcode == "0010011" or opcode == "0011011"){
        QString funct3 =ins.mid(17, 3);
        if(funct3 == "001" or funct3 == "101"){
            if(ui->comboBox->currentText() == "RV64I"){
                ui->textBrowser->setText(
                    "bin:" + ins + "\n" +
                    "funct6:" + ins.mid(0, 6) + "\n" +
                    "funct3:" + ins.mid(17, 3) + "\n" +
                    "rs1:" + ins.mid(12, 5) + "\n" +
                    "shamt:" + ins.mid(6, 6) + "\n" +
                    "rd:" + ins.mid(20, 5) + "\n" +
                    "opcode:" + opcode
                );
            }
            else{
                ui->textBrowser->setText(
                    "bin:" + ins + "\n" +
                    "funct6:" + ins.mid(0, 7) + "\n" +
                    "funct3:" + ins.mid(17, 3) + "\n" +
                    "rs1:" + ins.mid(12, 5) + "\n" +
                    "shamt:" + ins.mid(7, 5) + "\n" +
                    "rd:" + ins.mid(20, 5) + "\n" +
                    "opcode:" + opcode
                );
            }
        }
        else {
            ui->textBrowser->setText(
                "bin:" + ins + "\n" +
                "funct3:" + ins.mid(17, 3) + "\n" +
                "rs1:" + ins.mid(12, 5) + "\n" +
                "rd:" + ins.mid(20, 5) + "\n" +
                "imm:" + ins.mid(0, 12) + "\n" +
                "opcode:" + opcode
            );
        }
    }
    else if(opcode == "0010111" or opcode == "0110111"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "rd:" + ins.mid(20, 5) + "\n" +
            "imm:" + ins.mid(0, 20) + "\n" +
            "opcode:" + opcode
        );
    }
    else if(opcode == "1101111"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "rd:" + ins.mid(20, 5) + "\n" +
            "imm:" + ins.mid(0, 1) + ins.mid(12, 8) + ins.mid(11, 1) + ins.mid(1, 10) + "\n" +
            "opcode:" + opcode
        );
    }
    else if(opcode == "1100111"){
        ui->textBrowser->setText(
            "bin:" + ins + "\n" +
            "funct3:" + ins.mid(17, 3) + "\n" +
            "rs1:" + ins.mid(12, 5) + "\n" +
            "rd:" + ins.mid(20, 5) + "\n" +
            "imm:" + ins.mid(0, 12) + "\n" +
            "opcode:" + opcode
        );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

