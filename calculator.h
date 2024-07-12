#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();
    void backspaceClicked();
    void deleteClicked();
    void squareClicked();
    void lightClicked();
private:
    QLineEdit *display;
    double leftOperand;
    QString pendingOperator;
    bool waitingForOperand;
    QCheckBox* lightButton;
    void createLayout();
    void abortOperation();
    bool lightCheck;
    bool calculate(double rightOperand, const QString &operatorSymbol);
};

#endif // CALCULATOR_H
