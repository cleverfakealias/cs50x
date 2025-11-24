SELECT COUNT(m.id)
FROM movies m
JOIN ratings r on r.movie_id = m.id
WHERE r.rating = 10.0;
