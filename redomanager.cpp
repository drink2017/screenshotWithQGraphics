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

