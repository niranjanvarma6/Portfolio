import React from 'react';
import './../index.css';

function Home() {
  return (
    <div className="home-container">
      <header>
        <h1>Welcome to Cookers!</h1>
        <p>Discover new recipes and unleash your inner chef!</p>
      </header>
      <section className="intro">
        <h2>About Us</h2>
        <p>We are passionate about food and love sharing delicious recipes with our community. Whether you're a seasoned chef or a novice in the kitchen, we've got something for everyone!</p>
      </section>
      <section className="mission">
        <h2>Our Mission</h2>
        <p>Our mission is to inspire and empower people to cook homemade meals that are not only delicious but also nutritious. We believe that cooking should be fun, easy, and accessible to everyone.</p>
      </section>
    </div>
  );
}

export default Home;
