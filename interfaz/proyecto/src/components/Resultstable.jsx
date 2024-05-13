import React from 'react';
import styled from 'styled-components';

const TableContainer = styled.div`
    width: 100%;
    max-height: 800px; /* Altura fija para el contenedor */
    overflow-y: auto; /* Permitir desplazamiento vertical */
    overflow-x: auto; /* Permitir desplazamiento horizontal si es necesario */
    border: 1px solid #ddd;
`;

const Table = styled.table`
    width: 100%;
    border-collapse: collapse;
`;

const Th = styled.th`
    border: 1px solid #1e1c1c;
    padding: 8px;
    background-color: #fff080;
`;

const Td = styled.td`
    border: 1px solid #1e1c1c;
    padding: 8px;
`;

const ResultsTable = ({ results }) => {
    return (
        <TableContainer>
            <Table>
                <thead>
                <tr>
                    <Th>FDC ID</Th>
                    <Th>Brand</Th>
                    <Th>Description</Th>
                    <Th>Ingredients</Th>
                    <Th>Serving Size</Th>
                </tr>
                </thead>
                <tbody>
                {results.map((record, index) => (
                    <tr key={index}>
                        <Td>{record.fdc_id}</Td>
                        <Td>{record.brand}</Td>
                        <Td>{record.description}</Td>
                        <Td>{record.ingredients}</Td>
                        <Td>{record.servingsize}</Td>
                    </tr>
                ))}
                </tbody>
            </Table>
        </TableContainer>
    );
};

export default ResultsTable;
