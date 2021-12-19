#include "FontFile.h"

FontFile::FontFile(QString fontName)
{
    FontFile::fontName = fontName;
    changed = false;
}

void FontFile::Load(const QString loadFileName)
{
    QFile inputFile(loadFileName);

    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    FileName = loadFileName;

    QFileInfo fileInfo(loadFileName);

    filePath = fileInfo.absoluteDir().path();

    fontFileName = fileInfo.fileName();

    QByteArray inputArray(inputFile.readAll());

    QTextCodec *codec = QTextCodec::codecForName("CP-1250");

    QString decodedArray = codec->toUnicode(inputArray);

    QTextStream *inStream = new QTextStream(&decodedArray);

    ReadFromStream(inStream);

    if(fontName == "")
    {
        //Looks like file isn't Windows-1250, read as UTF-8
        delete inStream;

        CharactersArray.clear();

        inputFile.reset();

        inStream = new QTextStream(&inputFile);

        ReadFromStream(inStream);
    }

    delete inStream;

    inputFile.close();
    changed = false;
}

void FontFile::Save()
{
    Save(FileName);
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

    QString outString;

    QTextStream outStream(&outString);

    QTextCodec *codec = QTextCodec::codecForName("CP-1250");

    //Write basic info
    outStream <<  "[newfont]" << Qt::endl;
    outStream <<  fontName << Qt::endl;


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

    if(fileType == FileType::ANSI)
    {
        QByteArray convertedItems = codec->fromUnicode(outString);

        outFile.write(convertedItems);
    }
    else
    {
        QTextStream stream(&outFile);

        stream << outString;
    }

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
    if(index >= CharactersArray.size() || index < 0)
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

bool FontFile::HasItem (const QChar item)
{
    for (int i = 0; i < CharactersArray.length(); i++)
    {
        if(CharactersArray[i].GetCharacter() == item)
        {
            return true;
        }
    }

    return false;
}

void FontFile::ReadFromStream(QTextStream *inputStream)
{
    QString line;

    while (!(line = inputStream->readLine()).isNull())
    {
        //Read font parameters
        if(line == "[newfont]")
        {
            fontName = inputStream->readLine();
            fontColorImage = inputStream->readLine();
            fontAlphaImage = inputStream->readLine();
            characterHeight = inputStream->readLine().toUShort();
            horizontalGap = inputStream->readLine().toUShort();
        }

        //Read Characters
        if(line == "[char]")
        {
            AddCharacter(*inputStream);
        }
    }
}
