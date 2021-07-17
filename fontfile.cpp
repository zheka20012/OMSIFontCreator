#include "FontFile.h"

FontFile::FontFile(QString fontName)
{
    FontFile::fontName = fontName;
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
    if(fontAlphaImage == "")
    {
        throw std::runtime_error("File doens't have alpha image!");
    }

    QFile outFile(fileName);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw std::runtime_error("Can't open file " + fileName.toStdString() + "!");
    }

    QTextStream outStream(&outFile);

    //Write basic info
    outStream << "[newfont]" << Qt::endl;
    outStream << fontName << Qt::endl;


    QDir dir(fileName);
    QFileInfo outputDirInfo(fileName);
    QFileInfo *fileInfo;

    if(fontColorImage != "")
    {
        fileInfo = new QFileInfo(fontColorImage);

        if(fileInfo->fileName() != fontColorImage)
        {
            QFile::copy(fontColorImage, outputDirInfo.absoluteDir().path() + "\\" + fileInfo->fileName());
        }
        else
        {

            QFile::copy(filePath  + "/" + fontColorImage, outputDirInfo.absoluteDir().path() + "/" + fontColorImage);
        }

        outStream << dir.relativeFilePath(fontColorImage) << Qt::endl;

        delete fileInfo;
    }
    else
    {
        outStream << dir.relativeFilePath(fontAlphaImage) << Qt::endl;
    }

    fileInfo = new QFileInfo(fontAlphaImage);

    if(fileInfo->fileName() != fontAlphaImage)
    {
        QFile::copy(fontAlphaImage, outputDirInfo.absoluteDir().path() + "\\" + fileInfo->fileName());
    }
    else
    {

        QFile::copy(filePath  + "/" + fontAlphaImage, outputDirInfo.absoluteDir().path() + "/" + fontAlphaImage);
    }

    outStream << dir.relativeFilePath(fontAlphaImage) << Qt::endl;

    delete fileInfo;

    outStream << characterHeight << Qt::endl;
    outStream << horizontalGap << Qt::endl;
    outStream << Qt::endl << Qt::endl;

    //Write every character
    for(int i = 0; i< CharactersArray.size(); i++)
    {
        CharactersArray[i].Save(outStream);
    }

    outFile.close();


    fileInfo = new QFileInfo(fileName);

    filePath = fileInfo->absoluteDir().path();

    fontFileName = fileInfo->fileName();

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
