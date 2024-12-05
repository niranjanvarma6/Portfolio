import React, { useState } from 'react';
import foodsData from './../Data/foods.json'; // assuming you have your JSON file in the same directory
import { css } from '@emotion/react'
import styled from '@emotion/styled'

//for the categories of each ingredients (vegetables, legumes, etc.)
const ListElement = styled.li`
      background-color: white;
      border-radius: 10px;
      margin-bottom: 100px;
      font-size: 30px;
      text-align: center;
`
//styling for list of ingredients in each category (like carrots and tomoatos in vegetables category)
const IngredientList = styled.li`
      font-size: 16px;
      padding: 25px;
      max-width: 200px;
      min-width: 150px;
      display: inline-block;
      &:hover{
        font-weight: bold;
        background-color: white;
      }

      border: 1px solid #a2a2a2;
      background-color: #f4f4f4;
`



function FoodLinks() {
  const [selectedFood, setSelectedFood] = useState(null);

  const handleFoodClick = (food) => {
    setSelectedFood(food);
  };

  return (
    <div>
      <ul>
        {Object.entries(foodsData).map(([category, foods]) => (
          <ListElement key={category}>
            <h3>{((category.charAt(0).toUpperCase() + category.slice(1)).replace(/_/g,' '))}</h3>
            <ul>
              {foods.map((food) => (
                <IngredientList key={food}>
                  <a href={`ingredient/${food}`} onClick={() => handleFoodClick(food)}>
                    {food.charAt(0).toUpperCase() + food.slice(1)}
                  </a>
                </IngredientList>
              ))}
            </ul>
          </ListElement>
        ))}
      </ul>
      {selectedFood && (
        <div>
          <h2>Selected Food</h2>
          <p>You clicked on: {selectedFood}</p>
        </div>
      )}
    </div>
  );
}

export default FoodLinks;
