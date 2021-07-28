#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <cmath>
#include <QTime>
#include <ios>

#include <QMessageBox>

bool TryQSTR_to_float(QString a, float &ans){
    ans=0;
    bool findPoint=false;
    int j=-1;
    for (int i=0; i<a.length(); i++){
        if (a[i]=='.'){
            if (findPoint) return false;
            findPoint=true;
        }else{
            if (a[i]<'0' || a[i]>'9') return false;
            if (!findPoint){
                ans*=10;
                ans+=a[i].digitValue();
            }else{
                ans+=a[i].digitValue()*(float)std::pow(10, j--);
            }
        }
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->ui->centralwidget->setStyleSheet("background-color: black");
    this->setWindowTitle("CAT CASH");
    this->setFixedSize(800, 550);
    //Hello Window Inizilation
    EnterBut=new QPushButton(this);
    OpenRegBut=new QPushButton(this);
    EnterNum=new CleverEdit(this, "Enter number of the card(16 degits)");
    EnterNum->show();
    EnterNum->setGeometry(225, 240, 350, 30);
    EnterPassword=new CleverEdit(this, "Enter PIN(4 degits)");
    EnterPassword->show();
    EnterPassword->setGeometry(225, 280, 350, 30);
    OpenRegBut->setText("Register Account");
    EnterBut->setText("Enter");
    OpenRegBut->setGeometry(225, 320, 120, 40);
    EnterBut->setGeometry(355, 320, 220, 40);
    Res=QImage("Resources/RES.png");
    //MainMenu Window Inizilation
    BalBut=new PaintButton(this, Res, QRect(914+20, 0+20, 1040-40, 698-40));
    BalBut->setGeometry(90, 60, 300, 200);
    CashOutBut=new PaintButton(this, Res, QRect(914+20, 659+20, 1040-40, 698-40));
    CashOutBut->setGeometry(90, 280, 300, 200);
    CashInBut=new PaintButton(this, Res, QRect(1917+20, 659+20, 1040-40, 698-40));
    CashInBut->setGeometry(410, 60, 300, 200);
    HistBut=new PaintButton(this, Res, QRect(1917+20, 0+20, 1040-40, 698-40));
    HistBut->setGeometry(410, 280, 300, 200);
    connect(BalBut, SIGNAL(clicked()), this, SLOT(Bal_clicked()));
    connect(HistBut, SIGNAL(clicked()), this, SLOT(Hist_clicked()));
    connect(CashInBut, SIGNAL(clicked()), this, SLOT(CashIn_clicked()));
    connect(CashOutBut, SIGNAL(clicked()), this, SLOT(CashOut_clicked()));
    CashInBut->hide();
    CashOutBut->hide();
    HistBut->hide();
    BalBut->hide();
    connect(EnterBut, SIGNAL(clicked()), this, SLOT(Enter_clicked()));
    connect(OpenRegBut, SIGNAL(clicked()), this, SLOT(Reg_clicked()));
    //Hist elems Inizilation
    HistText=new QTextBrowser(this);
    HistText->setGeometry(225, 100, 350, 370);
    HistText->hide();
    //CashIn & CashOut elem Inizilation
    EnterSum=new CleverEdit(this, "Enter sum of operation(float number)");
    EnterSum->setGeometry(200, 240, 400, 100);
    EnterSum->hide();
    EnterSum->setFont(QFont(EnterSum->font().family(), 11, 5));
    TransactionBut=new QPushButton("Make the transaction", this);
    connect(TransactionBut, SIGNAL(clicked()), this, SLOT(Transaction_clicked()));
    TransactionBut->setGeometry(200, 350, 400, 50);
    TransactionBut->setFont(QFont(TransactionBut->font().family(), 15, 5));
    TransactionBut->hide();
    //Ballance elem Inizilation
    LabelClever=new CleverEdit(this, "");
    LabelClever->setGeometry(225, 100, 350, 370);
    LabelClever->setFont(QFont(LabelClever->font().family(), 30, 5));
    LabelClever->hide();
    //
    QString pathCards=QCoreApplication::applicationDirPath()+"/Resources/t3v.setting";
    std::fstream FileCards;
    FileCards.open(pathCards.toStdString(), std::ios::in);
    if (!FileCards.fail())
        for (; !FileCards.eof(); ){
            std::string Number;
            std::string password;
            float Bal;
            int kolHist;
            FileCards >> Number;
            FileCards >> password;
            FileCards >> Bal;
            FileCards >> kolHist;
            std::string temp;
            std::getline(FileCards, temp);
            std::vector <QString> History;
            for (int i=0; i<kolHist; i++){
                std::getline(FileCards, temp);
                History.push_back(QString::fromStdString(temp));
            }
            Cards.push_back(Card(QString::fromStdString(Number), QString::fromStdString(password), Bal, History));
        }
    FileCards.close();
}

void MainWindow::closeEvent(QCloseEvent *event){
    QString pathCards=QCoreApplication::applicationDirPath()+"/Resources/t3v.setting";
        std::fstream FileCards;
        FileCards.open(pathCards.toStdString(), std::ios::out | std::ios::trunc);

            for (int i=0; i<Cards.size(); i++){
                if (i!=0) FileCards << "\n";
                FileCards << Cards[i].Number.toStdString() << " " << Cards[i].Password.toStdString() << "\n";
                FileCards << Cards[i].Ballance << "\n" << Cards[i].History.size();
                for (int j=0; j<Cards[i].History.size(); j++)
                    FileCards << "\n" << Cards[i].History[j].toStdString();
            }

        FileCards.close();
    if (RoleWindow!="Main"){
        if (RoleWindow!="Hello"){
           RoleWindow="Main";
           CashInBut->show();
           CashOutBut->show();
           HistBut->show();
           BalBut->show();
           HistText->hide();
           LabelClever->hide();
           TransactionBut->hide();
           EnterSum->hide();
           this->EnterBut->setText("Log out");
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter p(this);
    if (RoleWindow=="Hello")
        p.drawImage(QRect(0,0, this->width(), this->height()), Res, QRect(0,0,912,833));
    else
        p.drawImage(QRect(0,0, this->width(), this->height()), Res, QRect(0,1412,1332,847));
}


bool MainWindow::DelSpaceInNumber(QString& a){
    QString t;
    for (int i=0; i<a.length(); i++){
        if (a[i]==' '){

        }else if(!a[i].isNumber()){
            return false;
        }else{
            t+=a[i];
        }
    }
    a=t;
    return true;
}

void MainWindow::Enter_clicked()
{
    if (RoleWindow=="Hello"){
        QString t1=EnterNum->text(), t2=EnterPassword->text();
        if (!DelSpaceInNumber(t1) || t1.length()!=16){
            QMessageBox::critical(this, "Error", "Wrong format of the number of the card");
            EnterNum->setFocus();
        }else if (t2=="" || !DelSpaceInNumber(t2) || t2.length()!=4){
            QMessageBox::critical(this, "Error", "Wrong format of the PIN");
            EnterPassword->setFocus();
        }else{
            int i=0;
            for (; i<Cards.size(); i++){
                if (Cards[i].Number==t1) break;
            }
            if (i!=Cards.size()){
                if (Cards[i].Password==t2){
                    CurrentCard=i;
                    RoleWindow="Main";
                    CashInBut->show();
                    CashOutBut->show();
                    BalBut->show();
                    HistBut->show();
                    EnterNum->hide();
                    OpenRegBut->hide();
                    EnterPassword->hide();
                    EnterBut->setText("Log out");
                    EnterBut->setGeometry(90, 490, 620, 40);
                    repaint();
                }else{
                    QMessageBox::critical(this, "Error", "Wrong PIN");
                    EnterPassword->setFocus();
                }
            }else{
                QMessageBox::critical(this, "Error", "Current number of the card non find in system");
                EnterNum->setFocus();
            }
        }
    }else if(RoleWindow=="Main"){
        this->close();
    }else{
        HistText->hide();
        EnterSum->hide();
        LabelClever->hide();
        TransactionBut->hide();
        this->EnterBut->setText("Log out");
        RoleWindow="Main";
        CashInBut->show();
        EnterBut->setGeometry(90, 490, 620, 40);
        CashOutBut->show();
        BalBut->show();
        HistBut->show();
    }
}

void MainWindow::Reg_clicked(){
    QString t1=EnterNum->text(), t2=EnterPassword->text();
    if (!DelSpaceInNumber(t1) || t1.length()!=16){
        QMessageBox::critical(this, "Error", "Wrong format of the number of the card");
        EnterNum->setFocus();
    }else if (t2=="" || !DelSpaceInNumber(t2) || t2.length()!=4){
        QMessageBox::critical(this, "Error", "Wrong format of the PIN");
        EnterPassword->setFocus();
    }else{
        int i=0;
        for (; i<Cards.size(); i++){
            if (Cards[i].Number==t1) break;
        }
        if (i==Cards.size()){
            QMessageBox GoodBoy;
            GoodBoy.setText("Welcome to family, don't forget password");
            GoodBoy.setStandardButtons(QMessageBox::Ok);
            GoodBoy.setDefaultButton(QMessageBox::Ok);
            GoodBoy.show();
            std::vector<QString> a;
            Cards.push_back(Card(t1, t2, 0, a));
            CurrentCard=Cards.size()-1;
            GoodBoy.exec();
            RoleWindow="Main";
            CashInBut->show();
            CashOutBut->show();
            BalBut->show();
            HistBut->show();
            EnterNum->hide();
            OpenRegBut->hide();
            EnterPassword->hide();
            EnterBut->setText("Log out");
            EnterBut->setGeometry(90, 480, 620, 40);
            repaint();
        }else{
            QMessageBox::critical(this, "Error", "Current number of the card already registred");
            EnterNum->setFocus();
        }
    }
}

void MainWindow::CashIn_clicked(){
    RoleWindow="CashIn";
    EnterBut->setText("Go back");
    CashInBut->hide();
    CashOutBut->hide();
    HistBut->hide();
    BalBut->hide();
    EnterSum->setText("");
    EnterBut->setGeometry(90, 480, 620, 40);
    EnterSum->show();
    TransactionBut->show();
    repaint();
}

void MainWindow::CashOut_clicked(){
    RoleWindow="CashOut";
    EnterBut->setText("Go back");
    CashInBut->hide();
    CashOutBut->hide();
    HistBut->hide();
    BalBut->hide();
    EnterSum->setText("");
    EnterBut->setGeometry(90, 480, 620, 40);
    EnterSum->show();
    TransactionBut->show();
    repaint();
}

void MainWindow::Transaction_clicked(){
    if (RoleWindow=="CashIn"){
        float temp=0;
        if (TryQSTR_to_float(EnterSum->text(), temp)){
            Cards[CurrentCard].Ballance+=temp;
            Enter_clicked();
            Cards[CurrentCard].History.push_back(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" to the account credited "+EnterSum->text()+"$");
        }else{
            QMessageBox::critical(this, "Error", "Wrong format of the sum");
            EnterSum->setFocus();
        }
    }else if (RoleWindow=="CashOut"){
        float temp=0;
        if (TryQSTR_to_float(EnterSum->text(), temp)){
            if (Cards[CurrentCard].Ballance<temp){
                QMessageBox::critical(this, "Error", "Not enough monry :(");
                EnterSum->setFocus();
            }else{
                Cards[CurrentCard].Ballance-=temp;
                Enter_clicked();

                Cards[CurrentCard].History.push_back(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" "+EnterSum->text()+"$ was debited from the account");
            }
        }else{
            QMessageBox::critical(this, "Error", "Wrong format of the sum");
            EnterSum->setFocus();
        }
    }
}

void MainWindow::Bal_clicked(){
    RoleWindow="Bal";

    EnterBut->setText("Go back");
    EnterBut->setGeometry(90, 480, 620, 40);
    CashInBut->hide();
    CashOutBut->hide();
    HistBut->hide();
    BalBut->hide();
    LabelClever->show();
    LabelClever->setText(QString::number(Cards[CurrentCard].Ballance, 'f', 2)+'$');
    LabelClever->setReadOnly(true);
    repaint();
}

void MainWindow::Hist_clicked(){
    RoleWindow="Hist";
    CashInBut->hide();
    CashOutBut->hide();
    HistBut->hide();
    BalBut->hide();
    HistText->show();
    HistText->setText("");
    for (int i=0; i<Cards[CurrentCard].History.size(); i++)
        HistText->insertPlainText(Cards[CurrentCard].History[i]+"\n");
    if (Cards[CurrentCard].History.size()==0) HistText->setText("History is empty");
    EnterBut->setText("Go back");
    repaint();
}

