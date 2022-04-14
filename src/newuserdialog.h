#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include "sql/sqlitedb.h"
#include <QDialog>

namespace Ui {
class NewUserDialog;
}

class NewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewUserDialog(QWidget *parent = nullptr, SQLiteDB* dbManager = nullptr);
    ~NewUserDialog();

private:
    Ui::NewUserDialog *ui;
    SQLiteDB* dbManager;

private slots:
    void createNewUser();
};

#endif // NEWUSERDIALOG_H
