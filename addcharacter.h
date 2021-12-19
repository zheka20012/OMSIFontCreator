#ifndef ADDCHARACTER_H
#define ADDCHARACTER_H

#include <QDialog>

namespace Ui {
class AddCharacter;
}

class AddCharacter : public QDialog
{
    Q_OBJECT

public:
    explicit AddCharacter(QWidget *parent = nullptr);
    ~AddCharacter();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddCharacter *ui;
};

#endif // ADDCHARACTER_H
