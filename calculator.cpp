#include "calculator.h"
#include <QtWidgets>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), leftOperand(0.0), waitingForOperand(true)
{
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setStyleSheet("background-color: gray;");
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);



    createLayout();

    setWindowTitle("Calculator");
    setFixedSize(300, 400);
}

void Calculator::createLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    lightButton = new QCheckBox("&Light");
    lightButton->setChecked(false);
    connect(lightButton,&QPushButton::clicked,this,&Calculator::lightClicked);

    mainLayout->addWidget(lightButton);
    mainLayout->addWidget(display);

    QGridLayout *buttonLayout = new QGridLayout;

    QStringList digitButtons{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int positions[10][2] = {{3, 1}, {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}};

    for (int i = 0; i < digitButtons.size(); ++i) {
        QPushButton *button = new QPushButton(digitButtons[i]);
        connect(button, &QPushButton::clicked, this, &Calculator::digitClicked);
        button->setStyleSheet("background-color: gray; color : white;");
        buttonLayout->addWidget(button, positions[i][0]+1, positions[i][1]);
    }

    QStringList operators{"+", "-", "×", "÷"};
    for (const QString &operatorSymbol : operators) {
        QPushButton *button = new QPushButton(operatorSymbol);
        connect(button, &QPushButton::clicked, this, &Calculator::operatorClicked);
        button->setStyleSheet("background-color: gray; color : white;");
        buttonLayout->addWidget(button, operators.indexOf(operatorSymbol), 3);
    }

    QPushButton *equalButton = new QPushButton("=");
    connect(equalButton, &QPushButton::clicked, this, &Calculator::equalClicked);
    equalButton->setStyleSheet("background-color: gray; color : white;");
    buttonLayout->addWidget(equalButton, 4, 3);

    QPushButton *clearButton = new QPushButton("C");
    connect(clearButton, &QPushButton::clicked, this, &Calculator::clearClicked);
    clearButton->setStyleSheet("background-color: gray; color : white;");
    buttonLayout->addWidget(clearButton, 0, 0);

    QPushButton *deleteButton = new QPushButton("<-");
    connect(deleteButton,&QPushButton::clicked,this,&Calculator::deleteClicked);
    deleteButton->setStyleSheet("background-color: gray; color : white;");
    buttonLayout->addWidget(deleteButton,0,2);

    QPushButton *squareButton = new QPushButton("x²");
    connect(squareButton,&QPushButton::clicked,this,&Calculator::squareClicked);
    squareButton->setStyleSheet("background-color: gray; color : white;");
    buttonLayout->addWidget(squareButton,0,1);
    mainLayout->addLayout(buttonLayout);
}
void Calculator::lightClicked()
{
    QPalette pal = palette();
    int nlight = lightButton->isChecked();
    if(nlight == false)
    {
        pal.setColor(backgroundRole(),QColor(Qt::black));
    }
    else
    {
        pal.setColor(backgroundRole(),QColor(Qt::white));
    }
    setPalette(pal);
}
void Calculator::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    int digitValue = clickedButton->text().toInt();

    if (display->text() == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }

    display->setText(display->text() + QString::number(digitValue));
}

void Calculator::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingOperator.isEmpty())
    {
        if (!calculate(operand, pendingOperator))
        {
            abortOperation();
            return;
        }
        display->setText(QString::number(leftOperand));
    }
    else
    {
        leftOperand = operand;
    }

    pendingOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::deleteClicked()
{
    QString text = display->text();
    text.chop(1);
    if(text.isEmpty())
    {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
}

void Calculator::squareClicked()
{
    double operand = display->text().toDouble();
    double result = operand*operand;
    display->setText(QString::number(result));
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    double operand = display->text().toDouble();

    if (!pendingOperator.isEmpty())
    {
        if (!calculate(operand, pendingOperator))
        {
            abortOperation();
            return;
        }
        pendingOperator.clear();
    }
    else
    {
        leftOperand = operand;
    }

    display->setText(QString::number(leftOperand));
    leftOperand = 0.0;
    waitingForOperand = true;
}

void Calculator::clearClicked()
{
    leftOperand = 0.0;
    pendingOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

void Calculator::abortOperation()
{
    clearClicked();
    display->setText("Error!!!");
}

bool Calculator::calculate(double rightOperand, const QString &operatorSymbol)
{
    if (operatorSymbol == "+") {
        leftOperand += rightOperand;
    } else if (operatorSymbol == "-") {
        leftOperand -= rightOperand;
    } else if (operatorSymbol == "×") {
        leftOperand *= rightOperand;
    } else if (operatorSymbol == "÷") {
        if (rightOperand == 0.0)
            return false;
        leftOperand /= rightOperand;
    }
    return true;
}
