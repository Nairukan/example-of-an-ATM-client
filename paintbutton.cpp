#include "paintbutton.h"

PaintButton::PaintButton(QWidget* parent, QImage Res, QRect Source):QPushButton(parent){
    this->Res=Res;
    this->Source=Source;
}

void PaintButton::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.drawImage(QRect(0,0, this->width(), this->height()), Res, Source);
}

CleverEdit::CleverEdit(QWidget* parent, QString Text):QLineEdit(parent)
{
    this->HelpText=new QLabel(this);
    this->HelpText->setText(Text);
    HelpText->setStyleSheet("color: lightGray");
    HelpText->setAlignment(Qt::AlignCenter);
    this->setGeometry(0,0, width(), height());
}

void CleverEdit::paintEvent(QPaintEvent* event){
    QPainter p(this);
    p.setPen(QPen(Qt::white, 2));
    p.setBrush(QBrush(Qt::black));
    p.drawRect(0,0, this->width(), this->height());
    if (this->text()!=""){
        HelpText->hide();
        p.setPen(Qt::white);
        QString VisibleText=this->text();
        for (;;){
            QFontMetrics fm(this->font());
            if (fm.width(VisibleText)<this->width()) break;
            VisibleText=VisibleText.left(VisibleText.length()-1);
        }
        p.setFont(this->font());
        QFontMetrics fm(this->font());
        p.drawText(QRect(2, 1, this->width()-4, this->height()-2),Qt::AlignCenter, VisibleText);
    }else{
        HelpText->show();
    }

}

void CleverEdit::resizeEvent(QResizeEvent *event){
    HelpText->resize(width(), height());
}
