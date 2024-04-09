#ifndef PENCOMMAND_H
#define PENCOMMAND_H

#include "command.h"

class penCommand : public command
{
public:
    penCommand();
    ~penCommand();

    void mousePressCommand(QMouseEvent *event) override;
    void mouseMoveCommand(QMouseEvent *event) override;
    void mouseReleaseCommand(QMouseEvent *event) override;
};

#endif // PENCOMMAND_H
