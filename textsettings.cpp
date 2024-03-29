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
