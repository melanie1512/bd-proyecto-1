import React, { useState, useEffect } from "react";
import styled from "styled-components";
import Sidebar from "./Sidebar.jsx";
import QueryEditor from "./QueryEditor.jsx";
import ResultsTable from "./Resultstable.jsx";
import { insertRecord, getRecords, deleteRecord, rangeSearch } from '../api.js';

const MainContainer = styled.div`
    display: flex;
    height: 100vh;
`;

const ContentContainer = styled.div`
    display: flex;
    flex-direction: column;
    flex: 1;
`;

const Button = styled.button`
    margin: 10px;
    padding: 10px 20px;
    background-color: #81cccc;
    color: white;
    border: none;
    border-radius: 5px;
    cursor: pointer;

    &:hover {
        background-color: #81cccc;
    }
`;

const Main = () => {
    const [tables, setTables] = useState(['Food Central']);
    const [selectedTable, setSelectedTable] = useState(null);
    const [query, setQuery] = useState('');
    const [results, setResults] = useState([]);

    useEffect(() => {
        if (selectedTable) {
            handleExecuteQuery();
        }
    }, [selectedTable]);

    const handleSelectTable = (table) => {
        setSelectedTable(table);
        setQuery(`SELECT * FROM ${table}`);
    };

    const handleQueryChange = (newQuery) => {
        setQuery(newQuery);
    };

    const handleExecuteQuery = async () => {
        if (query.toUpperCase().startsWith("INSERT INTO")) {
            try {
                const values = extractValuesFromQuery(query);
                await insertRecord(values);
                alert("Record inserted successfully");
                setQuery('');
                const records = await getRecords();
                setResults(records);
            } catch (error) {
                console.error('Error inserting record:', error);
            }
        } else if (query.toUpperCase().startsWith("DELETE FROM")) {
            try {
                const fdc_id = extractIdFromDeleteQuery(query);
                await deleteRecord(fdc_id);
                alert("Record deleted successfully");
                setQuery('');
                const records = await getRecords();
                setResults(records);
            } catch (error) {
                console.error('Error deleting record:', error);
            }
        } else if (query.toUpperCase().includes("WHERE FDC_ID BETWEEN")) {
            try {
                const { start, end } = extractRangeFromQuery(query);
                const records = await rangeSearch(start, end);
                setResults(records);
                alert("Range search executed successfully");
                setQuery('');
            } catch (error) {
                console.error('Error executing range search:', error);
            }
        } /*else if (query.toUpperCase().startsWith("CREATE TABLE")) {
            try {
                const { file_path, index_field } = extractTableInfoFromQuery(query);
                await createTable(file_path, index_field);
                alert("Table created and data loaded from CSV");
                setQuery('');
                const records = await getRecords();
                setResults(records);
            } catch (error) {
                console.error('Error creating table:', error);
            }
        }*/ else {
            try {
                const records = await getRecords();
                setResults(records);
            } catch (error) {
                console.error('Error fetching records:', error);
            }
        }
    };


    const extractValuesFromQuery = (query) => {
        // Analiza la consulta SQL para extraer los valores
        const valuesString = query.match(/\(([^)]+)\)/)[1];
        const valuesArray = valuesString.split(',').map(value => value.trim().replace(/['"]+/g, ''));
        const [fdc_id, brand, description, ingredients, servingsize] = valuesArray;
        return {
            fdc_id: Number(fdc_id),
            brand,
            description,
            ingredients,
            servingsize: parseFloat(servingsize)
        };
    };
    const extractIdFromDeleteQuery = (query) => {
        const match = query.match(/WHERE\s+fdc_id\s*=\s*(\d+)/i);
        if (match) {
            return Number(match[1]);
        }
        throw new Error('Invalid DELETE query: fdc_id not found');
    };
    const extractRangeFromQuery = (query) => {
        const match = query.match(/WHERE\s+fdc_id\s+BETWEEN\s+(\d+)\s+AND\s+(\d+)/i);
        if (match) {
            return {
                start: Number(match[1]),
                end: Number(match[2])
            };
        }
        throw new Error('Invalid range query: fdc_id range not found');
    };

    /*const extractTableInfoFromQuery = (query) => {
        const match = query.match(/FROM FILE\s+"([^"]+)"\s+USING INDEX HASH\s*\(\s*"([^"]+)"\s*\)/i);
        if (match) {
            return {
                file_path: match[1],
                index_field: match[2]
            };
        }
        throw new Error('Invalid CREATE TABLE query: file path or index field not found');
    };*/

    return (
        <MainContainer>
            <Sidebar tables={tables} onSelectTable={handleSelectTable} />
            <ContentContainer>
                <QueryEditor query={query} onQueryChange={handleQueryChange} onExecuteQuery={handleExecuteQuery} />
                <ResultsTable results={results} />
            </ContentContainer>
        </MainContainer>
    );
};

export default Main;
