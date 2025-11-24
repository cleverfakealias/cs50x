select AVG(energy)
from songs s
join artists a on a.id = s.artist_id and a.name = 'Drake';
