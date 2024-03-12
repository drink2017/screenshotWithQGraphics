#ifndef INFOLABEL_H
#define INFOLABEL_H

#include <QLabel>

class infoLabel : public QLabel
{
public:
    infoLabel();
    void updateInfoLabel();

private:
    void setLocation(QPoint selectStart,QPoint selectEnd);
    QString getInfo(QPoint selectStart,QPoint selectEnd);
};

#endif // INFOLABEL_H
