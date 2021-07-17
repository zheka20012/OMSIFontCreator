#include "FontFile.h"

FontFile::FontFile()
{

}

FontFile::FontFile(QString fontName)
{
    FontFile::fontName = fontName;
    changed = true;
}

void FontFile::Load(const QString fileName)
{
    QFile inputFile(fileName);

    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QFileInfo fileInfo(fileName);

    filePath = fileInfo.absoluteDir().path();

    fontFileName = fileInfo.fileName();

    QTextStream inStream(&inputFile);

    QString line;

    while (!(line = inStream.readLine()).isNull())
    {
        //Read font parameters
        if(line == "[newfont]")
        {
            fontName = inStream.readLine();
            fontColorImage = inStream.readLine();
            fontAlphaImage = inStream.readLine();
            characterHeight = inStream.readLine().toUShort();
            horizontalGap = inStream.readLine().toUShort();
        }

        //Read Characters
        if(line == "[char]")
        {
            AddCharacter(inStream);
        }
    }

    inputFile.close();
    changed = false;
}

void FontFile::Save()
{

}

void FontFile::Save(const QString fileName)
{
    QFile outFile(fileName);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

    QTextStream outStream(&outFile);

    //Write basic info
    outStream << "[newfont]" << Qt::endl;
    outStream << fontName << Qt::endl;
    outStream << fontColorImage << Qt::endl;
    outStream << fontAlphaImage << Qt::endl;
    outStream << characterHeight << Qt::endl;
    outStream << horizontalGap << Qt::endl;
    outStream << Qt::endl << Qt::endl;

    //Write every character
    for(int i = 0; i< CharactersArray.size(); i++)
    {
        CharactersArray[i].Save(outStream);
    }

    outFile.close();

    QFileInfo fileInfo(fileName);

    filePath = fileInfo.absoluteDir().path();

    fontFileName = fileInfo.fileName();

    //Save bitmaps

    changed = false;
}

int FontFile::GetItemIndex(const QChar letter)
{
    for(int i = 0; i < CharactersArray.size(); i++)
    {
        if(CharactersArray[i].GetCharacter() == letter)
        {
            return i;
        }
    }

    return -1;
}

void FontFile::AddCharacter(QTextStream &inStream)
{
    FontCharacter character(inStream);

    CharactersArray.push_back(character);

    changed = true;
}

int FontFile::AddCharacter(const QChar letter)
{
    if(GetItemIndex(letter) > 0)
    {
        return -1;
    }

    FontCharacter character(letter);

    CharactersArray.push_back(character);

    changed = true;

    return CharactersArray.size() - 1;
}

int FontFile::RemoveCharacter(const QChar letter)
{
    int itemIndex = GetItemIndex(letter);

    if(itemIndex < 0)
    {
        return -1;
    }

    return RemoveCharacter(itemIndex);
}

int FontFile::RemoveCharacter (int index)
{
    if(index >= CharactersArray.size())
    {
        return -1;
    }

    CharactersArray.removeAt(index);

    changed = true;
    return 0;
}

bool FontFile::IsChanged()
{
    return changed;
}

int FontFile::CharactersCount()
{
    return CharactersArray.size();
}

FontCharacter& FontFile::GetItem (const int index)
{
    return CharactersArray[index];
}
