import React from 'react';
import { Outlet, NavLink } from 'react-router-dom';
import './../Root.css';

export function Root(props) {
    const { children } = props;
    return (
        <>
            <header class="header">
                <h1 class="logo"><NavLink to="/" className="nav-link">COOKERS</NavLink></h1>
                    <ul class = "main-nav">


                        <li className="nav-item"><NavLink to="/choose_ingredient" className="nav-link">Choose Ingredient</NavLink></li>
                        <li className="nav-item"><NavLink to="/favorites" className="nav-link">View Favorites</NavLink></li>
                        <li className="nav-item"><NavLink to="/random_recipe" className="nav-link">Random Recipe</NavLink></li>


                        
                    
                    </ul>
            </header>
            <main className="main-content">{children || <Outlet />}</main>
        </>
    );
}

export default Root;
