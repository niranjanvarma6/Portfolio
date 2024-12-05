import React from "react";
import { Outlet, Link } from "react-router-dom";
import people from "./data/people.json";
import planets from "./data/planets.json";
import films from "./data/films.json";


const PeopleSidebar = () => {
  return (
    <div id="sideb">
      <nav>
        <ul>
          {Object.keys(people).map(id => (
            <li key={id}>
              <Link to={`/people/${id}`}>{people[id].name}</Link>
            </li>
          ))}
        </ul>
      </nav>
    </div>
  );
};

const PlanetSidebar = () => {
  return (
    <div id="sideb">
      <nav>
        <ul>
          {Object.keys(planets).map(id => (
            <li key={id}>
              <Link to={`/planets/${id}`}>{planets[id].name}</Link>
            </li>
          ))}
        </ul>
      </nav>
    </div>
  );
};

const FilmSidebar = () => {
  return (
    <div id="sideb">
      <nav>
        <ul>
          {Object.keys(films).map(id => (
            <li key={id}>
              <Link to={`/films/${id}`}>{films[id].title}</Link>
            </li>
          ))}
        </ul>
      </nav>
    </div>
  );
};




const ShowSidebar = () => {
  if (location.pathname === "/people/") {
    return <PeopleSidebar />;
  } else if (location.pathname === "/planets/") {
    return <PlanetSidebar />;
  } else if (location.pathname === "/films/") {
    return <FilmSidebar />;
  } else {
    return null;
  }  
};

export default function () {
  return (
    <>
      <nav id="navbar">
        <ul>
          <li><a href="/">Star Wars</a></li>
          <li><a href="/people/">People</a></li>
          <li><a href="/planets/">Planets</a></li>
          <li><a href="/films/">Films</a></li>
        </ul>
      </nav>

      {ShowSidebar()}
      <div id="detail">
        <Outlet />
      </div>
    </>
  );
}