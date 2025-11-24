SELECT m.title
FROM movies m
JOIN ratings r on r.movie_id = m.id
JOIN stars s on s.movie_id = m.id
JOIN people p on p.id = s.person_id and p.name = 'Chadwick Boseman'
ORDER BY r.rating DESC
LIMIT 5;
