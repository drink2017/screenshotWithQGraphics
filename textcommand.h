#ifndef TEXTCOMMAND_H
#define TEXTCOMMAND_H

#include "command.h"

class textCommand : public command
{
public:
    textCommand();
    ~textCommand();

    void mousePressCommand(QMouseEvent *event) override;
    void mouseMoveCommand(QMouseEvent *event) override;
    void mouseReleaseCommand(QMouseEvent *event) override;
};

#endif // TEXTCOMMAND_H
