#ifndef RECTCOMMAND_H
#define RECTCOMMAND_H

#include "command.h"

class rectCommand : public command
{
public:
    rectCommand();
    ~rectCommand();

    void mousePressCommand(QMouseEvent* event) override;
    void mouseMoveCommand(QMouseEvent *event) override;
    void mouseReleaseCommand(QMouseEvent *event) override;


};

#endif // RECTCOMMAND_H
