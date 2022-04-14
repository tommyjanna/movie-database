#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>

class SQLiteDB
{
public:
    enum RowEntry { entryID, entryTitle, entryYear, entryGenre, entryRuntime, entryActor, entryDirector, entryWriter };

    SQLiteDB(const QString& path);

    bool usernameExists(const QString& username);
    bool addUser(const QString& username, const QString& realName,
                 const QString& email);

    bool userHasWatched(const QString& username, const uint movieID);
    bool addUserWatched(const QString& username, const uint movieID);
    bool removeUserWatched(const QString& username, const uint movieID);

    void updateAllMoviesModel(QSqlQueryModel* model);
    void updateWatchedMoviesModel(const QString& username, QSqlQueryModel* model);
    void updateSearchMoviesModel(const QString& searchTerm, QSqlQueryModel* model);

    QList<QVariant> getMovieDataWithCreators(const uint movieID);
    QList<QVariant> getMovieData(const uint movieID);
    QString getMovieActors(const uint movieID);
    QString getMovieDirectors(const uint movieID);
    QString getMovieWriters(const uint movieID);
};

#endif // SQLITEDB_H
