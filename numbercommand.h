#ifndef NUMBERCOMMAND_H
#define NUMBERCOMMAND_H

#include "command.h"

class numberCommand : public command
{
public:
    numberCommand();
    ~numberCommand();

    void mousePressCommand(QMouseEvent *event) override;
    void mouseMoveCommand(QMouseEvent *event) override;
    void mouseReleaseCommand(QMouseEvent *event) override;
};

#endif // NUMBERCOMMAND_H
