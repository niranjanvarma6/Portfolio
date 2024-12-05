import React, { useState, useEffect } from 'react';
import { Navigate, useNavigate } from 'react-router-dom'; 
import foodsData from './../Data/foods.json';

function RandomRecipePage() {
  const [randomFood, setRandomFood] = useState(null);
  const navigate = useNavigate(); 

  useEffect(() => {
    const getRandomRecipe = () => {
      const categories = Object.keys(foodsData);
      const randomCategory = categories[Math.floor(Math.random() * categories.length)];
      const foodsInCategory = foodsData[randomCategory];
      const randomFood = foodsInCategory[Math.floor(Math.random() * foodsInCategory.length)];
      return randomFood;
    };

    const randomFood = getRandomRecipe();
    setRandomFood(randomFood);

    
    navigate(`/ingredient/${randomFood}`); 

  }, [history]); 

  return null;
}

export default RandomRecipePage;
