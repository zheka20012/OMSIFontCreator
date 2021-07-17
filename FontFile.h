#ifndef FONTFILE_H
#define FONTFILE_H
#include <QFile>
#include <QCoreApplication>
#include <fontcharacter.h>
#include <QFileDialog>

class FontFile
{
private:
    QString FileName;
    QVector<FontCharacter> CharactersArray;

    int GetItemIndex(const QChar letter);

    void AddCharacter(QTextStream &inStream);

    bool changed;

    QString fontName;
    QString fontFileName;
    QString filePath;
    QString fontAlphaImage;
    QString fontColorImage;
    unsigned short characterHeight;
    unsigned short horizontalGap;

public:


    FontCharacter& GetItem (const int index);

    FontFile();

    FontFile(const QString fontName);

    QString getFilePath() {return filePath;}
    QString getFontFileName() { return fontFileName != "" ? fontFileName : fontName;}

    QString getFontName() {return fontName;}
    void setFontName(QString newName) {fontName = newName; changed = true;}

    QString getFontAlphaImage() {return fontAlphaImage;}
    void setFontAlphaImage(QString newImage) {fontAlphaImage = newImage; changed = true;}

    QString getFontColorImage() {return fontColorImage;}
    void setFontColorImage(QString newImage) {fontColorImage = newImage; changed = true;}

    unsigned short getCharacterHeight() {return characterHeight;}
    void setCharacterHeight(unsigned short newValue) {characterHeight = newValue; changed = true;}

    unsigned short getHorizontalGap() {return horizontalGap;}
    void setHorizontalGap(unsigned short newValue) {horizontalGap = newValue; changed = true;}

    bool IsChanged();
    void setFileChanged(bool value) {changed = value;}

    void Load(const QString fileName);

    void Save(const QString fileName);

    void Save();


    int AddCharacter(const QChar letter);

    int RemoveCharacter(const QChar letter);

    int RemoveCharacter (int index);

    int CharactersCount();
};

#endif // FONTFILE_H
