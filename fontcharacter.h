#ifndef FONTCHARACTER_H
#define FONTCHARACTER_H
#include <QFile>
#include <QCoreApplication>

class FontCharacter
{
private:
    QChar Letter;
    ushort LeftPixel;
    ushort RightPixel;
    ushort TopPixel;

public:
    FontCharacter(const QChar letter);

    FontCharacter(QTextStream &inStream);

    ~FontCharacter();

    void SetCharacter(const QChar letter);

    void SetPixels(ushort leftPixel, ushort rightPixel, ushort topPixel);

    void Save(QTextStream& fileStream);

    QChar GetCharacter();
};

#endif // FONTCHARACTER_H
