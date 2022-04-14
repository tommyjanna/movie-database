# Movie Database

Data retrieved from [OMDb API](https://www.omdbapi.com/) using a [Python script](db/generate_sql_script.py) to generate a [SQL insertion script](db/insert_movies.sql).

Watch the [video desmonstration](https://www.youtube.com/watch?v=OIl7fhEWHyA) on YouTube.

* Design document: [`description.pdf`](description.pdf)

* C++ source that handles queries: [`src/sql/sqlitedb.cpp`](src/sql/sqlitedb.cpp)

* SQLite3 database: `db/movies.db`

Tommy Janna

March 30, 2022

## Building
The project can be built in the Qt Creator editor, or at the command line using qmake as follows.
```bash
qmake && make
./movie-database
```
Developed and tested using
* Debian 11
* Qt Version 5.15.3
* Qt Creator 7.0.0 (flathub)
