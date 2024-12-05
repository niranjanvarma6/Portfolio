import './index.css';
import React, { useState } from 'react';
import ReactDOM from 'react-dom';

function App(){
  //setting up main elements in display
  const [showDialog, setDialog] = useState(false);
  const [photoUrl, seturl] = useState('');
  const [caption, setcap] = useState('');
  const [photos, setphotos] = useState([]);
  const [error_, setError] = useState('');
  //opens dialog box
  const openDialog = () => {
    setDialog(true);
  };
  //closes dialog box and all elements
  const closeDialog = () => {
    setDialog(false);
    seturl('');
    setcap('');
    setError('');
  };
  //function used to accept input or give error message
  const Accept_ = () => {
    if (!(photoUrl && caption)) {
      setError('Please enter both URL and caption.');
    } 
    else {
      const newPhoto = {url: photoUrl, caption: caption};
      setphotos([newPhoto, ...photos]);
      closeDialog();
    }
  };
  //deletes selected photo by splicing the specific photo
  const deletePhoto = (index) => {
    const editphotos = [...photos];
    editphotos.splice(index, 1);
    setphotos(editphotos);
  };
  //main display for the page left-side consists of dialog box and right-side container has the array of photos
  return(
    <div id="app">
      <div id="left-side">
        <button onClick={openDialog}>Open photo entry dialog</button>
        {showDialog && (
          <div id="dialog">
            <input type="text" placeholder="Photo URL" value={photoUrl} onChange={(e) => seturl(e.target.value)}/>
            <br/>
            <input type="text" placeholder="Caption" value={caption} onChange={(e) => setcap(e.target.value)}/>
            <br/>
            <button onClick={Accept_}>Accept</button>
            <button onClick={closeDialog}>Cancel</button>
            <br/>
            <p id='errormessage'>{error_}</p>
          </div>
        )}
      </div>
      <div id="right-side">
      <div id="wrapper">
        {photos.map((photo, index) => (
          <div key={index} id="photo-post">
            <img src={photo.url} alt={photo.caption} style={{height: '300px' ,width: '200px',  marginTop: '5px', marginBottom: '5px', marginLeft: "5px", marginRight: "5px"}}/>
            <p>{photo.caption}</p>
            <button id="delete-button" onClick={() => deletePhoto(index)}>X</button>
          </div>
        ))}
      </div>
      </div>
    </div>
  );
};

ReactDOM.render(
  <React.StrictMode>
    <App /> {}
  </React.StrictMode>,
  document.getElementById('root') 
);
