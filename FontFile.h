#ifndef FONTFILE_H
#define FONTFILE_H
#include <QFile>
#include <QCoreApplication>
#include <fontcharacter.h>


class FontFile
{
private:
    QString FileName;
    QVector<FontCharacter> CharactersArray;

    int GetItemIndex(const QChar letter);

    void AddCharacter(QTextStream &inStream);
public:

    QString FontName;
    QString FontAlphaImage;
    QString FontColorImage;
    unsigned short CharacterHeight;
    unsigned short HorizontalGap;

    FontFile();

    FontFile(const QString fontName);

    ~FontFile();

    void Load(const QString fileName);

    void Save(const QString fileName);

    void Save();

    int AddCharacter(const QChar letter);

    int RemoveCharacter(const QChar letter);

    int RemoveCharacter (int index);
};

#endif // FONTFILE_H
