import React, { useState, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import films from "./data/films.json"; 

const Films = () => {
  const { id } = useParams();
  const [film, setFilm] = useState(null);

  useEffect(() => {
    if (films[id]) {
      setFilm(films[id]);
    } else {
      console.error(`Film with ID ${id} not found.`);
    }
  }, [id]);

  const ShowCharacters = () => {
    return (
      <ul>
        {film.characters.map(character => (
          <li key={character}>
            <a href={character.endsWith('/') ? character.slice(0, -1) : character}>{character}</a>
          </li>
        ))}
      </ul>
    );
  };
  
  const ShowPlanets = () => {
    return (
      <ul>
        {film.planets.map(planet => (
          <li key={planet}>
            <a href={planet.endsWith('/') ? planet.slice(0, -1) : planet}>{planet}</a>
          </li>
        ))}
      </ul>
    );
  };
  
  return (
    <div>
      {film ? (
        <div id = 'maincon'>
          <h1>{film.title}</h1>
          <p><b>Episode: </b> {film.episode_id}</p>
          <p><b>Opening Crawl: </b> {film.opening_crawl}</p>
          <p><b>Director: </b>{film.director}</p>
          <p><b>Producer: </b>{film.producer}</p>
          <p><b>Release Date: </b>{film.release_date}</p>
          <p><b>Characters: </b>{ShowCharacters()}</p>
          <p><b>Planets: </b>{ShowPlanets()}</p>
        </div>  
      ) : (
        <p id = 'errorm'>Film not found</p>
      )}
    </div>
  );
};
export default Films;