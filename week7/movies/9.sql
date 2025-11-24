SELECT p.name
FROM people p
JOIN stars s on s.person_id = p.id
JOIN movies m on m.id = s.movie_id and m.year = 2004
ORDER By p.birth ASC, p.name ASC;
