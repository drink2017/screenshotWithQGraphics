#ifndef COMMAND_H
#define COMMAND_H

#include <QMouseEvent>

class command
{
public:
    command();

    virtual void mousePressCommand(QMouseEvent* event) = 0;
    virtual void mouseMoveCommand(QMouseEvent* event) = 0;
    virtual void mouseReleaseCommand(QMouseEvent* event) = 0;
    virtual ~command();
};

#endif // COMMAND_H
