import React from "react";
import ReactDOM from "react-dom/client";
import {
  createBrowserRouter,
  RouterProvider,    
  BrowserRouter as Router,
  Routes,
  Route,
  NavLink,
  useLocation
} from "react-router-dom";
import App from "./App";
import Peoples from "./Peoples";
import Planets from "./Planets";
import Films from "./Films";
import "./index.css";


const router = createBrowserRouter([
  {
    path: "/",
    element: <App/>,
    children: [
      {
        path: "/",
        element: <div id = 'heading' >A long time ago, in a galaxy far, far away...</div>,
      },
      {
        path: "/people/:id/",
        element: <Peoples/>,
      },
      {
        path: "/people/",
        element: <div></div>,
      },
      {
        path: "/planets/:id/",
        element: <Planets/>,
      },
      {
        path: "/planets/",
        element: <div></div>,
      },      
      {
        path: "/films/:id/",
        element: <Films/>,
      },
      {
        path: "/films/",
        element: <div></div>,
      },
    ],
  },
]);


ReactDOM.createRoot(document.getElementById("root")).render(
  <React.StrictMode>
    <RouterProvider router={router} />
  </React.StrictMode>
);