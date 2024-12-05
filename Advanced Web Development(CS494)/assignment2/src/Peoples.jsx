import React, { useState, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import peoples from "./data/people.json";


const Peoples = () => {
  const { id } = useParams();
  const [people, setpeople] = useState(null);

  useEffect(() => {
    if (peoples[id]) {
      setpeople(peoples[id]);
    } else {
      console.error(`people with ID ${id} not found.`);
    }
  }, [id]);

  const ShowFilms = () => {
    return (
      <ul>
        {people.films.map(film => (
          <li key={film}>
            <a href={film.endsWith('/') ? film.slice(0, -1) : film}>{film}</a>
          </li>
        ))}
      </ul>
    );
  };  

  if (people) {
    return (
      <div id = 'maincon'>
        <h1>{people.name}</h1>
        <p><b>Height: </b>{people.height}</p>
        <p><b>Mass: </b>{people.mass}</p>
        <p><b>Hair Color: </b>{people.hair_color}</p>
        <p><b>Skin Color: </b>{people.skin_color}</p>
        <p><b>Eye Color: </b>{people.eye_color}</p>
        <p><b>Birth Year: </b>{people.birth_year}</p>
        <p><b>Gender: </b>{people.gender}</p>
        <p><b>Homeworld: </b><a href={people.homeworld}> {people.homeworld}</a></p>
        <p><b>Films: </b>{ShowFilms()}</p>
      </div>
    );
  } else {
    return <p id = 'errorm'>Person not found</p>;
  }
};

export default Peoples;