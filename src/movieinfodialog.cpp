#include "movieinfodialog.h"
#include "ui_movieinfodialog.h"

MovieInfoDialog::MovieInfoDialog(QWidget *parent, const uint movieID,
                                 const QString& username, SQLiteDB* dbManager) :
    QDialog(parent),
    ui(new Ui::MovieInfoDialog),
    movieID(movieID),
    username(username),
    dbManager(dbManager)
{
    ui->setupUi(this);

    connect(ui->watchButton, SIGNAL(clicked()), this, SLOT(toggleWatched()));

    updateUI();
}

MovieInfoDialog::~MovieInfoDialog()
{
    delete ui;
}

void MovieInfoDialog::updateUI()
{
    QList<QVariant> data = dbManager->getMovieDataWithCreators(movieID);

    setWindowTitle(data[SQLiteDB::RowEntry::entryTitle].toString());
    ui->titleLabel->setText(data[SQLiteDB::RowEntry::entryTitle].toString());
    ui->yearLabel->setText("Year released: " + data[SQLiteDB::RowEntry::entryYear].toString());
    ui->genreLabel->setText("Genres: " + data[SQLiteDB::RowEntry::entryGenre].toString());
    ui->runtimeLabel->setText("Runtime: " + data[SQLiteDB::RowEntry::entryRuntime].toString());
    ui->actorLabel->setText("Starring: " + data[SQLiteDB::RowEntry::entryActor].toString());
    ui->directorLabel->setText("Directed by: " + data[SQLiteDB::RowEntry::entryDirector].toString());
    ui->writerLabel->setText("Writen by: " + data[SQLiteDB::RowEntry::entryWriter].toString());

    if (dbManager->userHasWatched(username, movieID)) ui->watchButton->setText("Remove from watch list");
    else ui->watchButton->setText("Add to watch list");

}

void MovieInfoDialog::toggleWatched()
{
    if (dbManager->userHasWatched(username, movieID))
    {
        dbManager->removeUserWatched(username, movieID);
        ui->watchButton->setText("Add to watch list");
    }

    else
    {
        dbManager->addUserWatched(username, movieID);
        ui->watchButton->setText("Remove from watch list");
    }
}

