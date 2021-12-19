#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QFileDialog>
#include <FontFile.h>
#include <QMessageBox>
#include <QList>
#include <QAction>
#include <QResizeEvent>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private slots:
    void on_actionLoad_Alpha_Image_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_fontNameText_textChanged(const QString &arg1);

    void on_characterHeightEdit_valueChanged(int arg1);

    void on_horizontalGapEdit_valueChanged(int arg1);

    void on_charactersBox_currentIndexChanged(int index);

    void on_characterStartXEdit_valueChanged(int arg1);

    void on_characterEndXEdit_valueChanged(int arg1);

    void on_characterTopEdit_valueChanged(int arg1);

    void on_deleteItemButton_clicked();

    void on_addCharButton_clicked();

    void on_drawColorImage_stateChanged(int arg1);

    void on_drawAlphaImage_stateChanged(int arg1);

    void on_drawFinalImage_stateChanged(int arg1);

    void on_actionRemove_Color_Image_triggered();

    void on_fileFormatBox_currentIndexChanged(int index);

    void on_actionAbout_Qt_triggered();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    FontFile *file;

    void UpdateScene();
    void UpdateInterface();
    void UpdateCharacterInterface(int characterIndex);
    void UpdateTitle();
    int AskForFileSave();

    QString ShowSaveAsDialog();
};
#endif // MAINWINDOW_H
