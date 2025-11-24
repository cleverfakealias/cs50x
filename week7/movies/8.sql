SELECT p.name
FROM people p
JOIN stars s on s.person_id = p.id
JOIN movies m on m.id = s.movie_id and m.title = 'Toy Story';
