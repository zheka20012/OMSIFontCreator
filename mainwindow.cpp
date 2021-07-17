#include "mainwindow.h"
#include "./ui_mainwindow.h"
#define TITLE "OMSI Font Creator"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    on_actionNew_triggered();
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->graphicsView->setScene(scene);
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

    FontCharacter character = file->GetItem(characterIndex);

    ui->characterStartXEdit->setValue(character.getStartPixel());
    ui->characterEndXEdit->setValue(character.getEndPixel());
    ui->characterTopEdit->setValue(character.getTopPixel());

    ui->characterStartXEdit->blockSignals(false);
    ui->characterEndXEdit->blockSignals(false);
    ui->characterTopEdit->blockSignals(false);
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
    file = new FontFile("Untitled");

    ui->graphicsView->Init(file);
    UpdateInterface();
    UpdateScene();
    UpdateTitle();
}


void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Font File"), QDir::currentPath(), "Font File (*.oft)");

    if(filePath == "") return;

    if(file != NULL)
    {
        delete file;
    }

    file = new FontFile();
    file->Load(filePath);

    ui->graphicsView->Init(file);
    UpdateInterface();
    UpdateCharacterInterface(0);
    UpdateScene();
    UpdateTitle();
}


void MainWindow::on_actionQuit_triggered()
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

    QCoreApplication::quit();
}


void MainWindow::on_actionSave_triggered()
{
    if(file == NULL) return;

    QString savePath = file->getFilePath();

    if(savePath == "")
    {
        savePath = ShowSaveAsDialog();
    }

    if(savePath == "") return;

    file->Save(savePath);
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

void MainWindow::on_addCharButton_clicked()
{

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
}


void MainWindow::on_characterEndXEdit_valueChanged(int newValue)
{
    file->GetItem(ui->charactersBox->currentIndex()).setEndPixel(newValue);
    file->setFileChanged(true);
    UpdateScene();
    UpdateTitle();
}


void MainWindow::on_characterTopEdit_valueChanged(int newValue)
{
    file->GetItem(ui->charactersBox->currentIndex()).setTopPixel(newValue);
    file->setFileChanged(true);
    UpdateScene();
    UpdateTitle();
}

