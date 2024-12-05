import React, { useState, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import planets from "./data/planets.json";

const Planets = () => {
  const { id } = useParams();
  const [planet, setPlanet] = useState(null);

  useEffect(() => {
    if (planets[id]) {
      setPlanet(planets[id]);
    } else {
      console.error(`Planet with ID ${id} not found.`);
    }
  }, [id]);

  const ShowResidents = () => {
    return (
      <ul>
        {planet.residents.map(resident => (
          <li key={resident}>
            <a href={resident.endsWith('/') ? resident.slice(0, -1) : resident}>{resident}</a>
          </li>
        ))}
      </ul>
    );
  };
  

  const ShowFilms = () => {
    return (
      <ul>
        {planet.films.map(film => (
          <li key={film}>
            <a href={film.endsWith('/') ? film.slice(0, -1) : film}>{film}</a>
          </li>
        ))}
      </ul>
    );
  };
  
  if (planet) {
    return (
      <div id = 'maincon'>
        <h1>{planet.name}</h1>
        <p><b>Rotation Period: </b>{planet.rotation_period}</p>
        <p><b>Orbital Period: </b>{planet.orbital_period}</p>
        <p><b>Diameter: </b>{planet.diameter}</p>
        <p><b>Climate: </b>{planet.climate}</p>
        <p><b>Gravity: </b>{planet.gravity}</p>
        <p><b>Terrain: </b>{planet.terrain}</p>
        <p><b>Surface Water: </b>{planet.surface_water}</p>
        <p><b>Population: </b>{planet.population}</p>
        <p><b>Residents: </b>{ShowResidents()}</p>
        <p><b>Films: </b>{ShowFilms()}</p>
      </div>
    );
  } 
  else {
    return <p id = 'errorm'>Planet not found</p>;
  }  
};

export default Planets;