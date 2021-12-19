#ifndef FONTFILE_H
#define FONTFILE_H
#include <QFile>
#include <QCoreApplication>
#include <fontcharacter.h>
#include <QFileDialog>
#include <QTextCodec>

class FontFile
{
private:
    QString FileName;
    QVector<FontCharacter> CharactersArray;

    int GetItemIndex(const QChar letter);
    int selectedIndex;

    void AddCharacter(QTextStream &inStream);

    void ReadFromStream(QTextStream *inputStream);

    bool changed;

    QString fontName;
    QString fontFileName;
    QString filePath;
    QString fontAlphaImage;
    QString fontColorImage;
    unsigned short characterHeight;
    unsigned short horizontalGap;

public:
    enum FileType
    {
        UTF8,
        ANSI
    };

    FileType fileType = FileType::UTF8;

    FontCharacter& GetItem (const int index);

    FontFile(const QString fontName);

    QString getFilePath() {return filePath;}
    QString getFileName() {return FileName;}
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

    int getSelectedCharacterIndex() {return selectedIndex;}

    bool IsChanged();
    void setFileChanged(bool value) {changed = value;}

    bool HasItem (const QChar item);

    void Load(const QString fileName);

    void Save(const QString fileName);

    void Save();


    int AddCharacter(const QChar letter);

    int RemoveCharacter(const QChar letter);

    int RemoveCharacter (int index);

    int CharactersCount();
};

#endif // FONTFILE_H
