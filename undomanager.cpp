#include "undomanager.h"

undoManager* undoManager::instance = nullptr;

undoManager::undoManager()
{

}

undoManager* undoManager::getInstance(){
    if(instance == nullptr){
        instance = new undoManager();
    }
    return instance;
}

void undoManager::pushOrder(order *aOrder){
    undo.enqueue(aOrder);
}

order* undoManager::popOrder(){
    order* aOrder = undo.back();
    undo.pop_back();
    return aOrder;
}

bool undoManager::isEmpty(){
    if(undo.isEmpty()){
        return true;
    }else{
        return false;
    }
}
