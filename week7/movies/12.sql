SELECT m.title
FROM movies m
JOIN stars s1 on s1.movie_id = m.id
JOIN people p1 on p1.id = s1.person_id and p1.name = 'Jennifer Lawrence'
JOIN stars s2 on s2.movie_id = m.id
JOIN people p2 on p2.id = s2.person_id and p2.name = 'Bradley Cooper';
