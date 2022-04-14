#ifndef MOVIEINFODIALOG_H
#define MOVIEINFODIALOG_H

#include "sql/sqlitedb.h"
#include <QDialog>

namespace Ui {
class MovieInfoDialog;
}

class MovieInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MovieInfoDialog(QWidget *parent = nullptr,
                             const uint movieTitle = 0,
                             const QString& username = "",
                             SQLiteDB* dbManager = nullptr);
    ~MovieInfoDialog();

private slots:
    void toggleWatched();

private:
    void updateUI();

    Ui::MovieInfoDialog *ui;

    const uint movieID;
    const QString& username;
    SQLiteDB* dbManager;
};

#endif // MOVIEINFODIALOG_H
