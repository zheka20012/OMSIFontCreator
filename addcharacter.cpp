#include "addcharacter.h"
#include "ui_addcharacter.h"

AddCharacter::AddCharacter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCharacter)
{
    ui->setupUi(this);
}

AddCharacter::~AddCharacter()
{
    delete ui;
}

void AddCharacter::on_buttonBox_accepted()
{

}

