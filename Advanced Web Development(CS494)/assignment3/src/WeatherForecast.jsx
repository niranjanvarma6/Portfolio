import React from 'react';
import { css } from '@emotion/react';

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

const WeatherForecast = ({ weatherData, changeTemp }) => {
  return (
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
  );
};

export default WeatherForecast;
