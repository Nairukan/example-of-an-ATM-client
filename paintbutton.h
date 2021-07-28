#ifndef PAINTBUTTON_H
#define PAINTBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QImage>
#include <QFontMetrics>

class PaintButton : public QPushButton
{
    Q_OBJECT
public:
    PaintButton(QWidget* parent, QImage Res, QRect Source);
    void paintEvent(QPaintEvent* event);
    QImage Res;
    QRect Source;
};

class CleverEdit : public QLineEdit
{
    Q_OBJECT
public:
    CleverEdit(QWidget* parent=nullptr, QString Text="");
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    QLabel *HelpText;
    QString RoleWindow;
};

#endif // PAINTBUTTON_H
