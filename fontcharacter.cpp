#include "fontcharacter.h"

FontCharacter::~FontCharacter(){

}

FontCharacter::FontCharacter(const QChar letter)
{
    Letter = letter;
    LeftPixel = RightPixel = 0;
    TopPixel = 0;
}

FontCharacter::FontCharacter(QTextStream &inStream)
{
    Letter = inStream.readLine().at(0);
    LeftPixel = inStream.readLine().toUShort();
    RightPixel = inStream.readLine().toUShort();
    TopPixel = inStream.readLine().toUShort();
}

void FontCharacter::SetCharacter(const QChar letter)
{
    Letter = letter;
}

void FontCharacter::SetPixels(unsigned int leftPixel, unsigned int  rightPixel, unsigned int  topPixel)
{
    LeftPixel = leftPixel;
    RightPixel = rightPixel;
    TopPixel = topPixel;
}

void FontCharacter::Save(QTextStream &fileStream)
{
    fileStream << "[char]" << Qt::endl;
    fileStream << Letter << Qt::endl;
    fileStream << LeftPixel << Qt::endl;
    fileStream << RightPixel << Qt::endl;
    fileStream << TopPixel << Qt::endl;
    fileStream << Qt::endl;
}

QChar FontCharacter::GetCharacter()
{
    return Letter;
}
