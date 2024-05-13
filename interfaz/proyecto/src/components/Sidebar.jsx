import React from 'react';
import styled from 'styled-components';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faTable } from '@fortawesome/free-solid-svg-icons';

const SidebarContainer = styled.div`
    width: 200px;
    background: #ececec;
    padding: 10px;
`;

const TableList = styled.ul`
    list-style: none;
    padding: 0;
`;

const TableItem = styled.li`
    margin: 5px 0;
    padding: 5px;
    background: #81cccc;
    cursor: pointer;
    display: flex;
    align-items: center;
`;

const TableItemIcon = styled(FontAwesomeIcon)`
  margin-right: 20px;
`;
const Sidebar = ({ tables, onSelectTable }) => {
    return (
        <SidebarContainer>
            <h3>Tables</h3>
            <TableList>
                {tables.map((table) => (
                    <TableItem key={table} onClick={() => onSelectTable(table)}>
                        <TableItemIcon icon={faTable} />
                        {table}
                    </TableItem>
                ))}
            </TableList>
        </SidebarContainer>
    );
};

export default Sidebar;
