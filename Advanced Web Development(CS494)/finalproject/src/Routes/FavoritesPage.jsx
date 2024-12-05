import React, { useState, useEffect } from 'react';
import styled from '@emotion/styled';

const CenteredDiv = styled.div`
    text-align: center;
`;

const FavoriteList = styled.ul`
    list-style: none;
    padding: 0;
`;

const FavoriteRecipeItem = styled.li`
    margin: 20px auto;
    max-width: 400px;
    border: 1px solid #ccc;
    padding: 20px;
`;

const RecipeImage = styled.img`
    width: 200px;
    height: 200px;
`;

const RecipeLink = styled.a`
    display: block;
    margin-bottom: 10px;
    color: black;
    transition: color 0.3s;
    text-decoration: none;

    &:hover {
        color: skyblue;
    }
`;

const RemoveButton = styled.button`
    border-radius: 20px;
    background-color: red;
    color: #fff;
    padding: 10px 20px;
    border: none;
    cursor: pointer;
    transition: background-color 0.3s;

    &:hover {
        background-color: blue;
    }
`;

const Favorites = () => {
    const [favoriteRecipes, setFavoriteRecipes] = useState([]);

    useEffect(() => {
        const starredRecipes = Object.keys(localStorage)
            .filter(key => key.startsWith('recipe_'))
            .map(key => JSON.parse(localStorage.getItem(key)));
        setFavoriteRecipes(starredRecipes);
    }, []);

    const removeFromFavorites = (recipeId) => {
        localStorage.removeItem(`recipe_${recipeId}`);
        localStorage.removeItem(recipeId);

        const starredRecipes = Object.keys(localStorage)
            .filter(key => key.startsWith('recipe_'))
            .map(key => JSON.parse(localStorage.getItem(key)));
        setFavoriteRecipes(starredRecipes);
    };

    return (
        <CenteredDiv>
            <h1>Favorite Recipes</h1>
            <FavoriteList>
                {favoriteRecipes.map((recipe, index) => (
                    <FavoriteRecipeItem key={index}>
                        <RecipeImage src={recipe.imageUrl} alt={recipe.recipeName} />
                        
                        <RecipeLink
                            href={recipe.recipeUrl}
                            target="_blank"
                            rel="noopener noreferrer"
                        >
                            {recipe.recipeName}
                        </RecipeLink>
                        <RemoveButton
                            onClick={() => removeFromFavorites(recipe.recipeId)}
                        >
                            Remove from Favorites
                        </RemoveButton>
                    </FavoriteRecipeItem>
                ))}
            </FavoriteList>
        </CenteredDiv>
    );
};

export default Favorites;