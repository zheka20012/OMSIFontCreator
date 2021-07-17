#ifndef FONTCHARACTER_H
#define FONTCHARACTER_H
#include <QFile>
#include <QCoreApplication>

class FontCharacter
{
private:
    QChar Letter;
    unsigned int  LeftPixel;
    unsigned int  RightPixel;
    unsigned int  TopPixel;

public:
    FontCharacter(const QChar letter);

    FontCharacter(QTextStream &inStream);

    ~FontCharacter();

    void SetCharacter(const QChar letter);

    void SetPixels(unsigned int  leftPixel, unsigned int  rightPixel, unsigned int  topPixel);

    unsigned int getStartPixel() {return LeftPixel;}
    void setStartPixel(unsigned int value) {LeftPixel = value;}
    unsigned int getEndPixel() {return RightPixel;}
    void setEndPixel(unsigned int value) {RightPixel = value;}
    unsigned int getTopPixel() {return TopPixel;}
    void setTopPixel(unsigned int value) {TopPixel = value;}

    void Save(QTextStream& fileStream);

    QChar GetCharacter();
};

#endif // FONTCHARACTER_H
