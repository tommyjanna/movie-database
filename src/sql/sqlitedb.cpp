#include "sqlitedb.h"

SQLiteDB::SQLiteDB(const QString& path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    const bool success = db.open();

    if (!success)
    {
       qDebug() << "Database connection error:" << db.lastError();
    }
}

bool SQLiteDB::usernameExists(const QString& username)
{
    QSqlQuery query;

    // Don't use SELECT *, order is undefined
    query.prepare("SELECT username, real_name, email "
                  "FROM users "
                  "WHERE username=(:username)");
    query.bindValue(":username", username);
    query.exec();

    return query.next();
}

bool SQLiteDB::addUser(const QString& username, const QString& realName,
                       const QString& email)
{
    QSqlQuery query;

    query.prepare("INSERT INTO users "
                  "VALUES (:username, :realName, :email)");
    query.bindValue(":username", username);
    query.bindValue(":realName", realName);
    query.bindValue(":email", email);

    return query.exec();
}

bool SQLiteDB::userHasWatched(const QString& username, const uint movieID)
{
    QSqlQuery query;

    query.prepare("SELECT username "
                  "FROM users_has_watched "
                  "WHERE username=(:username) AND movie_id=(:movieID)");
    query.bindValue(":username", username);
    query.bindValue(":movieID", movieID);
    query.exec();

    return query.next();
}

bool SQLiteDB::addUserWatched(const QString& username, const uint movieID)
{
    QSqlQuery query;

    query.prepare("INSERT INTO users_has_watched "
                  "VALUES (:username, :movieID)");
    query.bindValue(":username", username);
    query.bindValue(":movieID", movieID);

    return query.exec();
}

bool SQLiteDB::removeUserWatched(const QString& username, const uint movieID)
{
    QSqlQuery query;

    query.prepare("DELETE FROM users_has_watched "
                  "WHERE username=(:username) AND movie_id=(:movieID)");
    query.bindValue(":username", username);
    query.bindValue(":movieID", movieID);

    return query.exec();
}

void SQLiteDB::updateAllMoviesModel(QSqlQueryModel* model)
{
    model->clear();
    model->setQuery("SELECT movie_id, title, year, genre, runtime "
                    "FROM movies");
}

void SQLiteDB::updateWatchedMoviesModel(const QString& username, QSqlQueryModel* model)
{
    QSqlQuery query;

    query.prepare("SELECT movie_id, title, year, genre, runtime "
                  "FROM movies "
                  "NATURAL JOIN users_has_watched "
                  "WHERE username=(:username)");
    query.bindValue(":username", username);
    query.exec();

    model->clear();
    model->setQuery(query);
}

void SQLiteDB::updateSearchMoviesModel(const QString& searchTerm, QSqlQueryModel* model)
{
    QSqlQuery query;

    query.prepare("SELECT movie_id, title, year, genre, runtime "
                  "FROM movies "
                  "NATURAL JOIN (SELECT * "
                                "FROM creators "
                                "NATURAL JOIN (SELECT * "
                                              "FROM movies_acted_by "
                                              "UNION SELECT * "
                                              "FROM movies_directed_by "
                                              "UNION SELECT * "
                                              "FROM movies_written_by) "
                                "WHERE LOWER(full_name) LIKE LOWER((:searchTerm))) "
                  "UNION SELECT movie_id, title, year, genre, runtime "
                  "FROM movies "
                  "WHERE LOWER(title) LIKE LOWER((:searchTerm))");
    query.bindValue(":searchTerm", "%" + searchTerm + "%");
    query.exec();

    model->clear();
    model->setQuery(query);
}

QList<QVariant> SQLiteDB::getMovieDataWithCreators(const uint movieID)
{
    QList<QVariant> result = getMovieData(movieID);
    result.append(QVariant(getMovieActors(movieID)));
    result.append(QVariant(getMovieDirectors(movieID)));
    result.append(QVariant(getMovieWriters(movieID)));

    return result;
}

QList<QVariant> SQLiteDB::getMovieData(const uint movieID)
{
    QSqlQuery query;
    QList<QVariant> result;

    query.prepare("SELECT movie_id, title, year, genre, runtime "
                  "FROM movies "
                  "WHERE movie_id=(:movieID)");
    query.bindValue(":movieID", movieID);
    query.exec();

    if (query.next())
    {
        result.append(query.value(0));
        result.append(query.value(1));
        result.append(query.value(2));
        result.append(query.value(3));
        result.append(query.value(4));
    }

    return result;
}

QString SQLiteDB::getMovieActors(const uint movieID)
{
     QSqlQuery query;
     QString result;

     query.prepare("SELECT full_name "
                   "FROM movies_acted_by "
                   "NATURAL JOIN movies "
                   "NATURAL JOIN creators "
                   "WHERE movie_id=(:movieID)");
     query.bindValue(":movieID", movieID);
     query.exec();

     while (query.next())
     {
         result.append(query.value(0).toString() + ", ");
     }

     // Remove the trailing comma and space
     result.chop(2);
     return result;
}

QString SQLiteDB::getMovieDirectors(const uint movieID)
{
    QSqlQuery query;
    QString result;

    query.prepare("SELECT full_name "
                  "FROM movies_directed_by "
                  "NATURAL JOIN movies "
                  "NATURAL JOIN creators "
                  "WHERE movie_id=(:movieID)");
    query.bindValue(":movieID", movieID);
    query.exec();

    while (query.next())
    {
        result.append(query.value(0).toString() + ", ");
    }

    result.chop(2);
    return result;
}

QString SQLiteDB::getMovieWriters(const uint movieID)
{
    QSqlQuery query;
    QString result;

    query.prepare("SELECT full_name "
                  "FROM movies_written_by "
                  "NATURAL JOIN movies "
                  "NATURAL JOIN creators "
                  "WHERE movie_id=(:movieID)");
    query.bindValue(":movieID", movieID);
    query.exec();

    while (query.next())
    {
        result.append(query.value(0).toString() + ", ");
    }

    result.chop(2);
    return result;
}
