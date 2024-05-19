// api.js
export async function getRecords() {
    try {
        const response = await fetch('http://localhost:8080/traversal', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json'
            }
        });
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return await response.json(); // Aquí se espera y maneja el JSON correctamente
    } catch (error) {
        console.error('Error fetching records:', error);
        throw error;
    }
}
export async function insertRecord(record) {
    try {
        console.log('Insert Record:', record); // Agregar registro de depuración
        const response = await fetch('http://localhost:8080/insert', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(record)
        });
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return await response.text();
    } catch (error) {
        console.error('Error inserting record:', error);
        throw error;
    }
}
// api.js
export async function deleteRecord(fdc_id) {
    try {
        const response = await fetch('http://localhost:8080/delete', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ fdc_id })
        });
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return await response.text();
    } catch (error) {
        console.error('Error deleting record:', error);
        throw error;
    }
}
// api.js
export async function rangeSearch(start, end) {
    try {
        const response = await fetch('http://localhost:8080/range_search', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ start, end })
        });
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return await response.json();
    } catch (error) {
        console.error('Error fetching range search records:', error);
        throw error;
    }

}

/*export async function createTable(file_path, index_field) {
    try {
        const response = await fetch('http://localhost:8080/create_table', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ file_path, index_field })
        });
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return await response.text();
    } catch (error) {
        console.error('Error creating table:', error);
        throw error;
    }
}*/


