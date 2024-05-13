// QueryEditor.jsx
import React from 'react';
import styled from 'styled-components';

const QueryEditorContainer = styled.div`
    display: flex;
    flex-direction: column;
    padding: 10px;
`;

const TextArea = styled.textarea`
    flex: 1;
    padding: 10px;
    font-size: 16px;
    margin-bottom: 12px;
`;

const Button = styled.button`
    padding: 10px 20px;
    background-color: #81cccc;
    color: #1e1c1c;
    border: none;
    border-radius: 5px;
    cursor: pointer;

    &:hover {
        background-color: #81cccc;
    }
`;

const QueryEditor = ({ query, onQueryChange, onExecuteQuery }) => {
    const handleInputChange = (event) => {
        onQueryChange(event.target.value);
    };

    const handleExecute = () => {
        onExecuteQuery();
    };

    return (
        <QueryEditorContainer>
            <TextArea value={query} onChange={handleInputChange} />
            <Button onClick={handleExecute}>Execute Query</Button>
        </QueryEditorContainer>
    );
};

export default QueryEditor;
