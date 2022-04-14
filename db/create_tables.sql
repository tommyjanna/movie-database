DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS movies;
DROP TABLE IF EXISTS creators;
DROP TABLE IF EXISTS users_has_watched;
DROP TABLE IF EXISTS movies_directed_by;
DROP TABLE IF EXISTS movies_written_by;
DROP TABLE IF EXISTS movies_acted_by;

CREATE TABLE users(
	username TEXT PRIMARY KEY NOT NULL,
	real_name TEXT NOT NULL,
	email TEXT NOT NULL);

CREATE TABLE movies(
	movie_id INTEGER PRIMARY KEY AUTOINCREMENT,
	title TEXT NOT NULL,
	year INT NOT NULL,
	genre TEXT NOT NULL,
	runtime TEXT NOT NULL);

CREATE TABLE creators(
	creator_id INTEGER PRIMARY KEY AUTOINCREMENT,
	full_name TEXT NOT NULL);

CREATE TABLE users_has_watched(
	username TEXT NOT NULL,
	movie_id INTEGER,
	PRIMARY KEY(username, movie_id),
	FOREIGN KEY(username) REFERENCES users(username),
	FOREIGN KEY(movie_id) REFERENCES movies(movie_id));

CREATE TABLE movies_directed_by(
	movie_id INTEGER,
	creator_id INTEGER,
	PRIMARY KEY(movie_id, creator_id),
	FOREIGN KEY(movie_id) REFERENCES movies(movie_id),
	FOREIGN KEY(creator_id) REFERENCES creators(creator_id));
	
CREATE TABLE movies_written_by(
	movie_id INTEGER,
	creator_id INTEGER,
	PRIMARY KEY(movie_id, creator_id),
	FOREIGN KEY(movie_id) REFERENCES movies(movie_id),
	FOREIGN KEY(creator_id) REFERENCES creators(creator_id));

CREATE TABLE movies_acted_by(
	movie_id INTEGER,
	creator_id INTEGER,
	PRIMARY KEY(movie_id, creator_id),
	FOREIGN KEY(movie_id) REFERENCES movies(movie_id),
	FOREIGN KEY(creator_id) REFERENCES creators(creator_id));
