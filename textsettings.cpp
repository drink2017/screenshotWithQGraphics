#include "textsettings.h"

textSettings::textSettings()
{

}

void textSettings::setTextSize(int textSize){
    this->textSize = textSize;
}

int textSettings::getTextSize(){
    return textSize;
}

void textSettings::setTextColor(QColor textColor){
    this->textColor = textColor;
}

QColor textSettings::getTextColor(){
    return textColor;
}

void textSettings::setNumberColor(QColor numberColor){
    this->numberColor = numberColor;
}

QColor textSettings::getNumberColor(){
    return numberColor;
}

void textSettings::setNumberSize(int numberSize){
    this->numberSize = numberSize;
}

int textSettings::getNumberSize(){
    return numberSize;
}
