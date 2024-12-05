import React, { useState } from 'react';
import { css } from '@emotion/react';

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

const CityInputBox = ({ onSearch, setSearches }) => {
  const [searchQuery, setSearchQuery] = useState('');

  const handleSearch = async () => {
    try {
      const apiKey = '3e39cabacaa1a235322aac1ecbbaa78c';
      const url = `http://api.openweathermap.org/geo/1.0/direct?q=${searchQuery}&limit=1&appid=${apiKey}`;
      const response = await fetch(url);
      if (!response.ok) {
        throw new Error('Failed to get data of place');
      }
      const data = await response.json();
      if (data.length > 0) {
        const { lat, lon, name, state, country } = data[0];
        setSearches([{ cityName: name, stateName: state, countryName: country, lat, lon }]);
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
};

export default CityInputBox;
