#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "movieinfodialog.h"
#include "newuserdialog.h"
#include "sql/sqlitedb.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    enum StackView { viewLogin, viewMovie };
    enum Tabs { tabAll, tabWatched, tabSearch };

    Ui::MainWindow* ui;

    QString currentUser;

    SQLiteDB dbManager;

    QSqlQueryModel* allMoviesModel;
    QSqlQueryModel* watchedMoviesModel;
    QSqlQueryModel* searchMoviesModel;

private slots:
    void login();
    void logout();
    void openNewUserDialog();
    void openMovieInfoDialog(const QModelIndex& index);
    void searchMovies();
};
#endif // MAINWINDOW_H
