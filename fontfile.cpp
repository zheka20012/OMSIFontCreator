#include "fontfile.h"

FontFile::FontFile()
{

}

FontFile::~FontFile()
{

}

FontFile::FontFile(QString fontName)
{
    FontName = fontName;
}

void FontFile::Load(const QString fileName)
{
    QFile inputFile(fileName);

    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream inStream(&inputFile);

    QString line;

    while (!(line = inStream.readLine()).isNull())
    {
        //Read font parameters
        if(line == "[newfont]")
        {
            FontName = inStream.readLine();
            FontColorImage = inStream.readLine();
            FontAlphaImage = inStream.readLine();
            CharacterHeight = inStream.readLine().toUShort();
            HorizontalGap = inStream.readLine().toUShort();
        }

        //Read Characters
        if(line == "[char]")
        {
            AddCharacter(inStream);
        }
    }

    inputFile.close();
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
    outStream << FontName << Qt::endl;
    outStream << FontColorImage << Qt::endl;
    outStream << FontAlphaImage << Qt::endl;
    outStream << CharacterHeight << Qt::endl;
    outStream << HorizontalGap << Qt::endl;
    outStream << Qt::endl << Qt::endl;

    //Write every character
    for(int i = 0; i< CharactersArray.size(); i++)
    {
        CharactersArray[i].Save(outStream);
    }

    outFile.close();
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
}

int FontFile::AddCharacter(const QChar letter)
{
    if(GetItemIndex(letter) > 0)
    {
        return -1;
    }

    FontCharacter character(letter);

    CharactersArray.push_back(character);

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

    return 0;
}
