# Thread system calls
**Benchmarking de la recherche par thread:**
- En général, la recherche de la valeur min et max par les threads est plus performante que la recherche séquentielle.
- Mais dans le cas d'une recherche avec un seul thread, la recherche séquentielle se voit etre plus performente car la recherche par thread inclut le temps de création de thread par le code.
- A partir de 2 threads, le temps de recherche se voit être divisé par deux que la recherche séquentielle.
- Mais au dela de la valeur maximale du nombre de coeur logique du système, la recherche par thread inclut un temps d'attente jusqu'à ce qu'un thread se libère.
- Ainsi, au-delà du nombre de cœurs logiques du processeur, le parallélisme commence à devenir contre-productive à cause de l'overhead lié à la gestion des threads et à la concurrence excessive des threads sur le tableau.
