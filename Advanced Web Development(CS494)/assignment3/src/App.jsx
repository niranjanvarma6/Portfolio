import React, { useState, useEffect } from 'react';
import { css } from '@emotion/react';
import axios from 'axios';
import { useParams } from 'react-router-dom';

const navbar = css`
  display: flex;
  align-items: center;
  background-color: blue;
  padding: 10px;
  width: 100%;
  top: 0;
  position: fixed;
`;

const inputbox = css`
  label {
    display: block;
    margin-right: 10px;
  }

  input {
    padding: 5px;
    margin-right: 10px;
  }

  button {
    padding: 10px 10px;
    cursor: pointer;
  }
`;

const forecasts = css`
  border: 1px solid black;
  border-radius: 7px;
  padding: 15px;
  margin-bottom: 20px;
  text-align: center;
  p {
    font-family: arial;
  }
  &:first-of-type {
    margin-top: 69px;
  }
`;

function LocationInputBox({ onSearch }) {
  const [searchQuery, setSearchQuery] = useState('');

  const handleSearch = async () => {
    try {
      const apiKey = '3e39cabacaa1a235322aac1ecbbaa78c';
      const url = `http://api.openweathermap.org/geo/1.0/direct?q=${searchQuery}&limit=1&appid=${apiKey}`;
      const response = await axios.get(url);
      const data = response.data;
      if (data.length > 0) {
        const { lat, lon } = data[0];
        onSearch(lat, lon);
      } else {
        throw new Error('No place found');
      }
    } catch (error) {
      console.error(error);
      return error.message;
    }
  };

  return (
    <div css={navbar}>
      <div css={inputbox}>
        <label htmlFor="searchInput" style={{ color: 'pink' }}>
          Enter city, state, and country code:
        </label>
        <input
          type="text"
          id="searchInput"
          value={searchQuery}
          onChange={(e) => setSearchQuery(e.target.value)}
          placeholder="Enter a place"
        />
      </div>
      <button onClick={handleSearch} style={{ marginLeft: '10px', marginTop: '15px' }}>
        Search
      </button>
    </div>
  );
}

function WeatherIcon({ iconCode }) {
  return <img src={`http://openweathermap.org/img/w/${iconCode}.png`} alt="Weather Icon" />;
}

function changeTemp(a) {
  return ((a - 273.15) * 9) / 5 + 32;
}

function App() {
  const [weatherData, setWeatherData] = useState([]);
  const [error, setError] = useState(null);
  const { lat, lon } = useParams();

  const fetchWeatherData = async (lat, lon) => {
    try {
      const apiKey = '3e39cabacaa1a235322aac1ecbbaa78c';
      const url = `https://api.openweathermap.org/data/2.5/forecast?lat=${lat}&lon=${lon}&appid=${apiKey}`;
      const response = await axios.get(url);
      const data = response.data;
      setWeatherData(data.list);
    } catch (error) {
      console.error(error);
      setError(error.message);
    }
  };

  useEffect(() => {
    if (lat && lon) {
      fetchWeatherData(lat, lon);
    }
  }, [lat, lon]);

  return (
    <div>
      <LocationInputBox onSearch={fetchWeatherData} />
      <div>
        {error ? (
          <p>Error: {error}</p>
        ) : (
          <div>
            {weatherData.map((forecast, index) => (
              <div key={index} css={forecasts}>
                <p>Time: {forecast.dt_txt}</p>
                <p>Forecast: {forecast.weather[0].description}</p>
                <p>Probability of Precipitation: {forecast.pop * 100}%</p>
                <p>Min Temperature: {changeTemp(forecast.main.temp_min).toFixed(2)} °F</p>
                <p>Max Temperature: {changeTemp(forecast.main.temp_max).toFixed(2)} °F</p>
                {forecast.weather[0].icon && <WeatherIcon iconCode={forecast.weather[0].icon} />}
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}

export default App;
