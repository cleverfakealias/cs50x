SELECT m.title, r.rating
FROM movies m
JOIN ratings r on r.movie_id = m.id and m.year = 2010 and r.rating IS NOT NULL
ORDER BY r.rating DESC, m.title ASC;
