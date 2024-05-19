import React from "react";
import AVLList from "./components/AVLList.jsx";
import './App.css'; // Si tienes estilos globales

const App = () => {
    return (
        <div className="container">
            <h1>Resultados de la Base de Datos</h1>
            <AVLList />
        </div>
    );
};

export default App;
