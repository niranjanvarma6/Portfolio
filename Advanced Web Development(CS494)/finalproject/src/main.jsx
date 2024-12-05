import React from 'react';
import ReactDOM from 'react-dom';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';

import ErrorPage from './Routes/ErrorPage';
import Home from './Routes/Home';
import FavoritesPage from './Routes/FavoritesPage';
import ChooseIngredient from './Routes/ChooseIngredient';
import RecipeOfTheDay from './Routes/RandomRecipe';
import Root from './Routes/Root';


import './index.css';
import Recipes from './Routes/Recipes';
import RandomRecipe from './Routes/RandomRecipe';

const App = () => (
  <Router>
    <Routes>
      <Route path="/" element={<Root />}>
        <Route index element={<Home />} />
        <Route path="favorites" element={<FavoritesPage />} />
        <Route path="choose_ingredient" element={<ChooseIngredient />} />
        <Route path="random_recipe" element={<RandomRecipe />} />
        <Route path="ingredient/:ingredient" element={<Recipes />} />
        

        <Route path="*" element={<ErrorPage />} />
      </Route>
    </Routes>
  </Router>
);

ReactDOM.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
  document.getElementById('root')
);
