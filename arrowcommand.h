#ifndef ARROWCOMMAND_H
#define ARROWCOMMAND_H

#include "command.h"

class arrowCommand : public command
{
public:
    arrowCommand();
    ~arrowCommand();

    void mousePressCommand(QMouseEvent *event) override;
    void mouseMoveCommand(QMouseEvent *event) override;
    void mouseReleaseCommand(QMouseEvent *event) override;
};

#endif // ARROWCOMMAND_H
