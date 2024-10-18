#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    btnNums = {
        {Qt::Key_0,ui->btnNum0},
        {Qt::Key_1,ui->btnNum1},
        {Qt::Key_2,ui->btnNum2},
        {Qt::Key_3,ui->btnNum3},
        {Qt::Key_4,ui->btnNum4},
        {Qt::Key_5,ui->btnNum5},
        {Qt::Key_6,ui->btnNum6},
        {Qt::Key_7,ui->btnNum7},
        {Qt::Key_8,ui->btnNum8},
        {Qt::Key_9,ui->btnNum9}
    };

    foreach (auto btn, btnNums)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSign,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double res = 0;
    if(operands.size() == 2 && opcodes.size() > 0) {
        double num1 = operands.front().toDouble();
        operands.pop_front();
        double num2 = operands.front().toDouble();
        operands.pop_front();

        QString op = opcodes.front();
        opcodes.pop_front();

        if(op == "+") res = num1 + num2;
        else if(op == "-") res = num1 - num2;
        else if(op == "x") res = num1 * num2;
        else if(op == "÷") res = num1 / num2;

        operands.push_back(QString::number(res));
    }
    return QString::number(res);
}

void MainWindow::btnNumClicked()
{

    QString digit = qobject_cast<QPushButton*>(sender())->text();
    if(digit == "0" && operand == "0") digit = "";
    if(operand == "0" && digit != "0") operand = "";

    operand += digit;

    ui->display->setText(operand);
}

void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
        operand += qobject_cast<QPushButton*>(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    operand = operand.left(operand.length() - 1);
    ui->display->setText(operand);
}


void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
    ui->display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode = qobject_cast<QPushButton*>(sender())->text();
    if(operand != "") {
        operands.push_back(operand);
        operand = "";

        opcodes.push_back(opcode);
    }

    QString res = calculation();

    ui->display->setText(res);
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand != "") {
        double res = operand.toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton*>(sender())->text();

        if(op == "%") res /=100.0;
        else if(op == "1/x") res = 1/res;
        else if(op == "x²") res *=res;
        else if(op == "√") res = sqrt(res);
        else if(op == "+/-") res = -res;

        operand = QString::number(res);
        ui->display->setText(QString::number(res));
    }
}


void MainWindow::on_btnEqual_clicked()
{
    if(operand != "") {
        operands.push_back(operand);
        operand = "";
    }

    QString res = calculation();

    ui->display->setText(res);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey, btnNums.keys()) {
        if(event->key() == btnKey)
            btnNums[btnKey]->animateClick();
    }
}


void MainWindow::on_btnClear_clicked()
{
    operands.clear();
    opcodes.clear();
    operand = "";
    opcode = "";
    ui->display->setText("0");
}

