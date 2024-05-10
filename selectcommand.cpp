#include "selectcommand.h"
#include "screenshotview.h"
#include "commandmanager.h"

selectCommand::selectCommand()
{

}

void selectCommand::mousePressCommand(QMouseEvent* event){
    screenshotView::getInstance()->setSelectStart(event->pos());
    screenshotView::getInstance()->setSelectEnd(event->pos());
    screenshotView::getInstance()->setCurrentSelectItem(new selectItem());
    selectItem* currentSelectItem = screenshotView::getInstance()->getCurrentSelectItem();
    currentSelectItem->setPath(screenshotView::getInstance()->getShadowPath(event->pos(),event->pos()));
    screenshotView::getInstance()->getScene()->addItem(currentSelectItem);

    screenshotView::getInstance()->setInfo(new infoLabel());
    screenshotView::getInstance()->getInfo()->show();
}

void selectCommand::mouseMoveCommand(QMouseEvent *event){
    screenshotView::getInstance()->setSelectEnd(event->pos());
    selectItem* currentSelectItem = screenshotView::getInstance()->getCurrentSelectItem();
    currentSelectItem->setPath(screenshotView::getInstance()->getShadowPath(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()));
    currentSelectItem->updateRectHandles();

    screenshotView::getInstance()->getInfo()->updateInfoLabel();
}

void selectCommand::mouseReleaseCommand(QMouseEvent *event){
    Q_UNUSED(event);
    selectItem* currentSelectItem = screenshotView::getInstance()->getCurrentSelectItem();
    currentSelectItem->setHover(true);
    currentSelectItem->setFinishSelect();
    commandManager::getInstance()->setSelectingArea(false);
    screenshotView::getInstance()->setControl(new controlWidget(screenshotView::getInstance()));
    commandManager::getInstance()->connectToControlWidget();
    screenshotView::getInstance()->getControl()->show();

    screenshotView::getInstance()->setCursor(Qt::ArrowCursor);
}

selectCommand::~selectCommand(){

}
