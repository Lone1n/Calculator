#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

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
private:
    QLineEdit *display;
    double leftOperand;
    QString pendingOperator;
    bool waitingForOperand;

    void createLayout();
    void abortOperation();
    bool calculate(double rightOperand, const QString &operatorSymbol);
};

#endif // CALCULATOR_H
