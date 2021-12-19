#include "mainwindow.h"
#include "./ui_mainwindow.h"
#define TITLE "OMSI Font Creator"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    file = new FontFile("Untitled");
    ui->graphicsView->Init(file);

    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->graphicsView->setScene(scene);
    UpdateInterface();
    UpdateScene();
    UpdateTitle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateScene()
{
    ui->graphicsView->Redraw(ui->charactersBox->currentIndex());
}

void MainWindow::UpdateInterface()
{
    ui->fontNameText->blockSignals(true);
    ui->characterHeightEdit->blockSignals(true);
    ui->horizontalGapEdit->blockSignals(true);
    ui->charactersBox->blockSignals(true);

    ui->fontNameText->setText(file->getFontName());

    ui->characterHeightEdit->setValue(file->getCharacterHeight());

    ui->horizontalGapEdit->setValue(file->getHorizontalGap());

    ui->charactersBox->clear();

    for (int i = 0; i < file->CharactersCount(); i++)
    {
        ui->charactersBox->addItem(file->GetItem(i).GetCharacter());
    }

    ui->fontNameText->blockSignals(false);
    ui->characterHeightEdit->blockSignals(false);
    ui->horizontalGapEdit->blockSignals(false);
    ui->charactersBox->blockSignals(false);
}

void MainWindow::UpdateCharacterInterface(int characterIndex)
{
    ui->characterStartXEdit->blockSignals(true);
    ui->characterEndXEdit->blockSignals(true);
    ui->characterTopEdit->blockSignals(true);
    ui->charactersBox->blockSignals(true);

    ui->charactersBox->setCurrentIndex(characterIndex);

    FontCharacter character = file->GetItem(characterIndex);

    ui->characterStartXEdit->setValue(character.getStartPixel());
    ui->characterEndXEdit->setValue(character.getEndPixel());
    ui->characterTopEdit->setValue(character.getTopPixel());

    ui->characterStartXEdit->blockSignals(false);
    ui->characterEndXEdit->blockSignals(false);
    ui->characterTopEdit->blockSignals(false);
    ui->charactersBox->blockSignals(false);
}

void MainWindow::UpdateTitle()
{
    setWindowTitle(QString(TITLE) + QString(" - ") + file->getFontFileName() + (file->IsChanged() ? "*" : ""));
}

int MainWindow::AskForFileSave()
{
    QMessageBox* askBox = new QMessageBox(this);
    askBox->setText(tr("You have unsaved changes! Do you want to save them before quit?"));
    askBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    int ret = askBox->exec();

    return ret;
}


void MainWindow::on_actionLoad_Alpha_Image_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Alpha Image"), QDir::currentPath(), "Image Files (*.bmp *png *jpg)");

    if(filePath == "") return;

    QFileInfo fileInfo(filePath);

    file->setFontAlphaImage(fileInfo.fileName());

    UpdateInterface();
    UpdateScene();
    UpdateTitle();
}


void MainWindow::on_actionNew_triggered()
{    
    if(file != nullptr && file->IsChanged())
    {
        int outValue = AskForFileSave();

        switch (outValue) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
        }
    }

    delete file;

    file = new FontFile("Untitled");

    ui->graphicsView->Init(file);
    UpdateInterface();
    UpdateScene();
    UpdateTitle();
}


void MainWindow::on_actionOpen_triggered()
{
    if(file->IsChanged())
    {
        int outValue = AskForFileSave();

        switch (outValue) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
        }
    }

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Font File"), QDir::currentPath(), "Font File (*.oft)");

    if(filePath == "") return;

    if(file != NULL)
    {
        delete file;
    }

    file = new FontFile("");
    file->Load(filePath);



    ui->graphicsView->Init(file);
    UpdateInterface();
    if(file->CharactersCount() != 0) UpdateCharacterInterface(0);
    UpdateScene();
    UpdateTitle();
}


void MainWindow::on_actionQuit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionSave_triggered()
{
    if(file == NULL) return;

    QString savePath = file->getFileName();

    if(savePath == "")
    {
        savePath = ShowSaveAsDialog();
    }

    if(savePath == "") return;

    try
    {
        file->Save(savePath);
    }
    catch (const std::exception& e)
    {
        QMessageBox error(QMessageBox::Warning, "Error saving file!", e.what(), QMessageBox::Ok);
        error.exec();
    }

    UpdateTitle();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString savePath = ShowSaveAsDialog();

    if(savePath == "") return;

    file->Save(savePath);
    UpdateTitle();
}

QString MainWindow::ShowSaveAsDialog()
{
    QString dir = file->getFilePath() == "" ? QDir::currentPath() : file->getFilePath();

    QString savePath = QFileDialog::getSaveFileName(this, tr("Save Font"), dir, tr("Font File (*.oft)"));

    return savePath;
}


void MainWindow::on_fontNameText_textChanged(const QString &newText)
{
    file->setFontName(newText);
    UpdateInterface();
}


void MainWindow::on_characterHeightEdit_valueChanged(int newValue)
{
    file->setCharacterHeight(newValue);
    UpdateTitle();
    UpdateScene();
}

void MainWindow::on_horizontalGapEdit_valueChanged(int newValue)
{
    file->setHorizontalGap(newValue);
    UpdateTitle();
    UpdateScene();
}


void MainWindow::on_charactersBox_currentIndexChanged(int index)
{
    UpdateCharacterInterface(index);
    ui->graphicsView->Redraw(index);
}


void MainWindow::on_characterStartXEdit_valueChanged(int newValue)
{
    file->GetItem(ui->charactersBox->currentIndex()).setStartPixel(newValue);
    file->setFileChanged(true);
    UpdateTitle();
    UpdateScene();
    UpdateCharacterInterface(ui->charactersBox->currentIndex());
}


void MainWindow::on_characterEndXEdit_valueChanged(int newValue)
{
    file->GetItem(ui->charactersBox->currentIndex()).setEndPixel(newValue);
    file->setFileChanged(true);
    UpdateScene();
    UpdateTitle();
    UpdateCharacterInterface(ui->charactersBox->currentIndex());
}


void MainWindow::on_characterTopEdit_valueChanged(int newValue)
{
    file->GetItem(ui->charactersBox->currentIndex()).setTopPixel(newValue);
    file->setFileChanged(true);
    UpdateScene();
    UpdateTitle();
    UpdateCharacterInterface(ui->charactersBox->currentIndex());
}

void MainWindow::closeEvent(QCloseEvent *event)

{
    if(file->IsChanged())
    {
        int outValue = AskForFileSave();

        switch (outValue) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        }
    }

    event->accept();

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->deleteItemButton->resize(ui->deleteItemButton->height(), ui->deleteItemButton->height());
}

void MainWindow::on_deleteItemButton_clicked()
{
    int lastIndex = ui->charactersBox->currentIndex();

    if(lastIndex < 0) return;

    file->RemoveCharacter(lastIndex);
    UpdateInterface();
    UpdateCharacterInterface(qMin(lastIndex, file->CharactersCount()-1));
    update();
}


void MainWindow::on_addCharButton_clicked()
{
    QMessageBox itemExistsBox(QMessageBox::Warning, "Error adding character!", "Font already have this character!", QMessageBox::Ok);

    if(ui->newCharField->text().length() != 1) return;

    QChar character = ui->newCharField->text().at(0);

    if(file->HasItem(character))
    {
        QMessageBox itemExistsBox(QMessageBox::Warning, "Error adding character!", "Font already have this character!", QMessageBox::Ok);
        return;
    }

    file->AddCharacter(character);
    UpdateInterface();
    UpdateCharacterInterface(file->CharactersCount()-1);
    ui->graphicsView->Redraw(file->CharactersCount()-1);
}



void MainWindow::on_drawColorImage_stateChanged(int arg1)
{
    ui->graphicsView->setDrawColor((bool)arg1);
    ui->drawAlphaImage->setChecked(ui->graphicsView->getDrawAlpha());
}


void MainWindow::on_drawAlphaImage_stateChanged(int arg1)
{
    ui->graphicsView->setDrawAlpha((bool)arg1);
    ui->drawColorImage->setChecked(ui->graphicsView->getDrawColor());
}


void MainWindow::on_drawFinalImage_stateChanged(int arg1)
{
    ui->graphicsView->setDrawFinal((bool)arg1);
}


void MainWindow::on_actionRemove_Color_Image_triggered()
{
    file->setFontColorImage("");
    ui->graphicsView->ForceRedraw();
}


void MainWindow::on_fileFormatBox_currentIndexChanged(int index)
{
    file->fileType = (FontFile::FileType)index;
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

