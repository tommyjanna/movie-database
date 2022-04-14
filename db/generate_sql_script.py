#!/usr/bin/env python3

'''
generate_sql_script.py
Generates an SQL script which inserts movie data into a database with existing
tables. The data is retrieved from the OMDb API (https://www.omdbapi.com).
Tommy Janna
Mar. 22, 2022
'''

import http.client
import json
import re

sql_filename = 'insert_movies.sql'
api_key = 'yourApiKeyHere'
movie_ids = [
        'tt0109830', 'tt1464335', 'tt0111161', 'tt0120915', 'tt0121765', 
        'tt0121766', 'tt0076759', 'tt0080684', 'tt0086190', 'tt2488496',
        'tt2527336', 'tt2527338', 'tt0120737', 'tt0167261', 'tt0167260',
        'tt3748528', 'tt0088763', 'tt0096874', 'tt0099088', 'tt0133093',
        'tt0234215', 'tt0242653', 'tt0110912', 'tt1375666', 'tt0816692',
        'tt0468569', 'tt7286456', 'tt0993846', 'tt1853728', 'tt1130884',
        'tt0068646', 'tt0071562', 'tt0099674', 'tt0361748', 'tt0099685',
        'tt0105236', 'tt0266697', 'tt0378194', 'tt3460252', 'tt0086250',
        'tt0075314', 'tt0114814', 'tt0407887', 'tt0114369', 'tt0209144',
        'tt0073486', 'tt4633694', 'tt0910970', 'tt0114709', 'tt0120363',
        'tt0435761', 'tt1979376', 'tt0110357', 'tt2380307', 'tt1049413',
        'tt0266543', 'tt0198781', 'tt0317705', 'tt0245429', 'tt0347149',
        'tt0096283', 'tt0097814', 'tt0876563', 'tt0240772', 'tt0349903',
        'tt0496806', 'tt0054135', 'tt0258463', 'tt0372183', 'tt0440963',
        'tt1119646', 'tt1411697', 'tt1951261', 'tt1232829', 'tt2294449',
        'tt0829482', 'tt0910936', 'tt2788710', 'tt0838283', 'tt0357413',
        'tt0415306', 'tt1386588', 'tt0120815', 'tt1798188', 'tt0298203',
        'tt0351283', 'tt0382932', 'tt0120623', 'tt0120855', 'tt0120587'
]


def main():
    sql_file = open(sql_filename, 'w')
    conn = http.client.HTTPSConnection('www.omdbapi.com', 443)

    # Counts for incremental ID keys (start at 1 in SQL)
    movie_count = 1
    creator_count = 1
    creator_id_for_name = {}

    for movie_id in movie_ids:
        conn.request('GET', f'/?i={movie_id}&apikey={api_key}')

        data_json = conn.getresponse().read().decode('utf-8')
        data_dict = json.loads(data_json)

        # Movie data
        title   = escape_quotes(data_dict['Title'])
        year    = escape_quotes(data_dict['Year'])
        genre   = escape_quotes(data_dict['Genre'])
        runtime = escape_quotes(data_dict['Runtime'])

        # Creator data
        directors = [escape_quotes(x) for x in data_dict['Director'].split(', ')]
        writers   = [escape_quotes(x) for x in data_dict['Writer'].split(', ')]
        actors    = [escape_quotes(x) for x in data_dict['Actors'].split(', ')]
        all_creators = directors + writers + actors

        # Create entry in movies table
        sql_file.write("INSERT INTO movies (title, year, genre,"
                       f" runtime) VALUES ('{title}', {year}, '{genre}',"
                       f" '{runtime}');\n")

        # Create entries for each creator
        for creator in all_creators:
            if not creator in creator_id_for_name:
                sql_file.write("INSERT INTO creators (full_name) VALUES"
                               f" ('{creator}');\n")
                creator_id_for_name[creator] = creator_count
                creator_count += 1

        # Create relational entry between each creator and the associated movie
        for director in directors:
            sql_file.write("INSERT INTO movies_directed_by(movie_id, creator_id)"
                           f" VALUES ({movie_count},"
                           f" {creator_id_for_name[director]});\n")
            
        for writer in writers:
            sql_file.write("INSERT INTO movies_written_by(movie_id, creator_id)"
                           f" VALUES ({movie_count},"
                           f" {creator_id_for_name[writer]});\n")

        for actor in actors:
            sql_file.write("INSERT INTO movies_acted_by(movie_id, creator_id)"
                           f" VALUES ({movie_count},"
                           f" {creator_id_for_name[actor]});\n")

        sql_file.write('\n')
        movie_count += 1

    sql_file.close()


def escape_quotes(input_str: str) -> str:
    # Prepends all single quote with SQL's escape character (another single quote)
    return re.sub(r"'", r"''", input_str)


if __name__ == '__main__':
    main()
