#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      dbManager("db/movies.db")
{
    ui->setupUi(this);

    // Start the UI at the login page
    ui->stackedWidget->setCurrentIndex(StackView::viewLogin);

    // Connect UI signals
    connect(ui->loginEdit, SIGNAL(returnPressed()), this, SLOT(login()));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(login()));

    connect(ui->logoutButton, SIGNAL(clicked()), this, SLOT(logout()));

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionNewUser, SIGNAL(triggered()), this, SLOT(openNewUserDialog()));

    connect(ui->actionAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

    connect(ui->allTable, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(openMovieInfoDialog(const QModelIndex&)));
    connect(ui->watchedTable, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(openMovieInfoDialog(const QModelIndex&)));
    connect(ui->searchTable, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(openMovieInfoDialog(const QModelIndex&)));

    connect(ui->searchEdit, SIGNAL(returnPressed()), this, SLOT(searchMovies()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(searchMovies()));

    ui->allTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->watchedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->searchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    allMoviesModel = new QSqlQueryModel();
    watchedMoviesModel = new QSqlQueryModel();
    searchMoviesModel = new QSqlQueryModel();

    // Load all movies model, this won't change during execution
    dbManager.updateAllMoviesModel(allMoviesModel);

    ui->allTable->setModel(allMoviesModel);
    ui->watchedTable->setModel(watchedMoviesModel);
    ui->searchTable->setModel(searchMoviesModel);

    // Hide the movie_id, but keep the data so it can be used for refinement queries.
    ui->allTable->setColumnHidden(SQLiteDB::RowEntry::entryID, true);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete allMoviesModel;
    delete watchedMoviesModel;
    delete searchMoviesModel;
}

void MainWindow::login()
{
    QString inputUsername = ui->loginEdit->text();

    const bool success = dbManager.usernameExists(inputUsername);

    if (success)
    {
        currentUser = inputUsername;

        dbManager.updateWatchedMoviesModel(currentUser, watchedMoviesModel);
        ui->watchedTable->setColumnHidden(SQLiteDB::RowEntry::entryID, true);

        ui->stackedWidget->setCurrentIndex(StackView::viewMovie);
    }
}

void MainWindow::logout()
{
    ui->stackedWidget->setCurrentIndex(StackView::viewLogin);
    currentUser = "";

    watchedMoviesModel->clear();
    searchMoviesModel->clear();
}

void MainWindow::openNewUserDialog()
{
    NewUserDialog nud(this, &dbManager);
    nud.setModal(true);
    nud.exec();
}

void MainWindow::openMovieInfoDialog(const QModelIndex& index)
{
    MovieInfoDialog mid(this, index.siblingAtColumn(SQLiteDB::RowEntry::entryID).data().toUInt(),
                        currentUser, &dbManager);
    mid.setModal(true);
    mid.exec();

    // Re-query watched list in case changes have been made
    dbManager.updateWatchedMoviesModel(currentUser, watchedMoviesModel);
    ui->watchedTable->setColumnHidden(SQLiteDB::RowEntry::entryID, true);
}

void MainWindow::searchMovies()
{
    dbManager.updateSearchMoviesModel(ui->searchEdit->text(), searchMoviesModel);
    ui->searchTable->setColumnHidden(SQLiteDB::RowEntry::entryID, true);

    // Automatically switch to the search tab
    ui->tabWidget->setCurrentIndex(Tabs::tabSearch);
}

