#ifndef ELLIPSECOMMAND_H
#define ELLIPSECOMMAND_H

#include "command.h"

class ellipseCommand : public command
{
public:
    ellipseCommand();
    ~ellipseCommand();

    void mousePressCommand(QMouseEvent *event) override;
    void mouseMoveCommand(QMouseEvent *event) override;
    void mouseReleaseCommand(QMouseEvent *event) override;
};

#endif // ELLIPSECOMMAND_H
