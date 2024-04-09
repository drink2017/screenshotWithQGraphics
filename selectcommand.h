#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include "command.h"

class selectCommand : public command
{
public:
    selectCommand();

    void mousePressCommand(QMouseEvent* event) override;
    void mouseMoveCommand(QMouseEvent* event) override;
    void mouseReleaseCommand(QMouseEvent* event) override;

    virtual ~selectCommand();
};

#endif // SELECTCOMMAND_H
