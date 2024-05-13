import React, { useState, useEffect } from "react";
import styled from "styled-components";

const ListContainer = styled.div`
  display: flex;
  flex-direction: column;
  align-items: center;
`;

const ListItem = styled.div`
  background-color: #f9c2ff;
  padding: 20px;
  margin: 8px;
  border-radius: 10px;
  width: 80%;
  display: flex;
  flex-direction: column;
  align-items: center;
`;

const ItemTitle = styled.h2`
  font-size: 18px;
  font-weight: bold;
`;

const ItemValue = styled.p`
  font-size: 16px;
`;

const AVLList = () => {
    const [data, setData] = useState([]);

    useEffect(() => {
        // Simular la obtención de datos de la base de datos
        const fetchData = async () => {
            // Aquí iría la lógica para obtener los datos de tu base de datos
            const results = [
                { id: "1", name: "Elemento 1", value: "Valor 1" },
                { id: "2", name: "Elemento 2", value: "Valor 2" },
                { id: "3", name: "Elemento 3", value: "Valor 3" },
            ];
            setData(results);
        };

        fetchData();
    }, []);

    return (
        <ListContainer>
            {data.map(item => (
                <ListItem key={item.id}>
                    <ItemTitle>{item.name}</ItemTitle>
                    <ItemValue>{item.value}</ItemValue>
                </ListItem>
            ))}
        </ListContainer>
    );
};

export default AVLList;
