#include "newuserdialog.h"
#include "ui_newuserdialog.h"

NewUserDialog::NewUserDialog(QWidget *parent, SQLiteDB* dbManager) :
    QDialog(parent),
    ui(new Ui::NewUserDialog),
    dbManager(dbManager)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(createNewUser()));
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}

void NewUserDialog::createNewUser()
{
    if (ui->usernameEdit->text().isEmpty() ||
        ui->realNameEdit->text().isEmpty() ||
        ui->emailEdit->text().isEmpty())
        return;

    dbManager->addUser(ui->usernameEdit->text(), ui->realNameEdit->text(),
                       ui->emailEdit->text());

    this->accept();
}
