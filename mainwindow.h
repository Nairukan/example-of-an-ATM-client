#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <vector>
#include <QTextBrowser>
#include "paintbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Card{
    QString Number;
    QString Password;
    float Ballance;
    std::vector <QString> History;
    Card(QString Number, QString Password, float Ballance, std::vector<QString> History){
        this->Number=Number;
        this->Password=Password;
        this->Ballance=Ballance;
        this->History=History;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){
        delete EnterNum;
        delete EnterPassword;
        delete EnterBut;
        delete OpenRegBut;
        delete CashInBut;
        delete CashOutBut;
        delete BalBut;
        delete HistBut;
        delete ui;
    }
    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent* event);
    bool DelSpaceInNumber(QString &a);
    CleverEdit* EnterNum, *EnterPassword, *EnterSum, *LabelClever;
    QPushButton* EnterBut, *OpenRegBut, *TransactionBut;
    PaintButton* CashInBut, *CashOutBut, *BalBut, *HistBut;
    QTextBrowser* HistText;
    int CurrentCard;
    QImage Res;
    QString RoleWindow="Hello";
private slots:
    void Enter_clicked();
    void Reg_clicked();
    void CashIn_clicked();
    void CashOut_clicked();
    void Bal_clicked();
    void Hist_clicked();
    void Transaction_clicked();
private:
    Ui::MainWindow *ui;
    std::vector <Card> Cards;
};
#endif // MAINWINDOW_H
