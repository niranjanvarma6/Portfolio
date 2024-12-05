import React, { useState, useEffect } from 'react';
import styled from '@emotion/styled';
import { useParams } from 'react-router-dom';
import { useNavigate } from 'react-router-dom';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faStar } from '@fortawesome/free-solid-svg-icons';

const CenteredDiv = styled.div`
    text-align: center;
`;

const RecipeList = styled.li`
    margin: 20px auto;
    max-width: 400px;
    border: 1px solid #ccc;
    padding: 20px;
    text-align: center;
`;

const RecipeImage = styled.img`
    display: block;
    margin: 0 auto 10px;
    width: 100%;
    max-width: 100%;
    height: auto;
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

const StarIcon = styled.span`
    cursor: pointer;
    color: ${(props) => (props.starred ? 'gold' : 'gray')};
    font-size: 2.5em;
`;

const Recipes = () => {
    const { ingredient } = useParams();
    const [recipes, setRecipes] = useState([]);
    const navigate = useNavigate();

    useEffect(() => {
        const fetchRecipes = async () => {
            try {
                const response = await fetch(`https://forkify-api.herokuapp.com/api/search?q=${ingredient}`);
                const data = await response.json();
                const recipesWithStars = data.recipes.map((recipe) => ({
                    ...recipe,
                    starred: localStorage.getItem(recipe.recipe_id) === 'true',
                    image_url: recipe.image_url,
                }));
                setRecipes(recipesWithStars);
            } catch (error) {
                console.error('Error fetching the recipes: ', error);
                navigate('/error_page');
            }
        };
        fetchRecipes();
    }, [ingredient, navigate]);

    const toggleStarred = (recipeId, imageUrl, recipeName) => {
        setRecipes((prevRecipes) => {
            const updatedRecipes = prevRecipes.map((recipe) => {
                if (recipe.recipe_id === recipeId) {
                    const newStarred = !recipe.starred;
                    localStorage.setItem(recipeId, newStarred.toString());
                    if (newStarred) {
                        localStorage.setItem(`recipe_${recipeId}`, JSON.stringify({
                            imageUrl: imageUrl,
                            recipeName: recipeName,
                            recipeId: recipeId,
                            recipeUrl: recipe.source_url,
                        }));
                    } else {
                        localStorage.removeItem(`recipe_${recipeId}`);
                    }
                    return { ...recipe, starred: newStarred };
                }
                return recipe;
            });
            return updatedRecipes;
        });
    };

    return (
        <CenteredDiv>
            <h1>Recipes for {ingredient}</h1>
            <ul style={{ listStyle: 'none', padding: 0 }}>
                {recipes.map((recipe, index) => (
                    <RecipeList key={index}>
                        <RecipeImage src={recipe.image_url} alt={recipe.title} />
                        
                        <RecipeLink href={recipe.source_url} target="_blank" rel="noopener noreferrer">
                            {recipe.title}
                        </RecipeLink>
                        <StarIcon
                            starred={recipe.starred}
                            onClick={() => toggleStarred(recipe.recipe_id, recipe.image_url, recipe.title)}
                        >
                            <FontAwesomeIcon icon={faStar} />
                        </StarIcon>
                    </RecipeList>
                ))}
            </ul>
        </CenteredDiv>
    );
};

export default Recipes;
