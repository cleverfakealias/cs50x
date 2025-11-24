SELECT AVG(r.rating) as AVERAGE_RATING
FROM ratings r
JOIN movies m on m.id = r.movie_id and m.year = 2012;
