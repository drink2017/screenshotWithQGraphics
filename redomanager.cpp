#include "redomanager.h"

redoManager* redoManager::instance = nullptr;

redoManager::redoManager()
{

}

redoManager* redoManager::getInstance(){
    if(instance == nullptr){
        instance = new redoManager();
    }
    return instance;
}

void redoManager::pushOrder(order *aOrder){
    redo.enqueue(aOrder);
}

bool redoManager::isEmpty(){
    if(redo.isEmpty()){
        return true;
    }else{
        return false;
    }
}

order* redoManager::popOrder(){
    order* aOrder = redo.back();
    redo.pop_back();
    return  aOrder;
}

void redoManager::clear(){
    redo.clear();
}















































