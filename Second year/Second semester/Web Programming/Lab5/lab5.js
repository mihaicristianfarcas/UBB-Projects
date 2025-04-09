$(document).ready(function() {
  // load data from local storage when page loads
  loadTableFromLocalStorage();
  
  // delete button click
  $(document).on('click', '.delete-btn', function() {
    $(this).closest('tr').remove();

    saveTableToLocalStorage();
  });
  
  // add button click
  $(document).on('click', '.add-btn', function() {
    const currentRow = $(this).closest('tr');
    const newRow = $('<tr class="editable">' +
      '<td><button class="delete-btn action-button">Delete</button></td>' +
      '<td contenteditable="true"></td>' +
      '<td contenteditable="true"></td>' +
      '<td contenteditable="true"></td>' +
      '<td contenteditable="true"></td>' +
      '<td contenteditable="true"></td>' +
      '<td><button class="add-btn action-button">Add</button></td>' +
      '</tr>');
    currentRow.after(newRow);

    saveTableToLocalStorage();
  });
  
  // cell editing
  $(document).on('input', '[contenteditable="true"]', function() {
    const row = $(this).closest('tr');
    const cells = row.find('td:not(:first-child):not(:last-child)');
    // let allFilled = true;
    
    // cells.each(function() {
    //   if (!$(this).text().trim()) {
    //     allFilled = false;
    //     return false;
    //   }
    // });
    
    // if (allFilled) {
    //   row.removeClass('editable').addClass('readonly');
    //   cells.attr('contenteditable', 'false');
    // }
    saveTableToLocalStorage();
  });
  
  function saveTableToLocalStorage() {
    const tableData = {
      headers: [],
      rows: []
    };
    
    // save headers
    $('#dynamicTable thead th').each(function(index) {
      // skip first and last columns
      if (index !== 0 && index !== $('#dynamicTable thead th').length - 1) {
        tableData.headers.push($(this).text());
      }
    });
    
    // save
    $('#dynamicTable tbody tr').each(function() {
      const rowData = [];
      $(this).find('td:not(:first-child):not(:last-child)').each(function() {
        rowData.push($(this).text());
      });
      
      // add class info for styling
      const isEditable = $(this).hasClass('editable');
      const isReadonly = $(this).hasClass('readonly');
      
      tableData.rows.push({
        data: rowData,
        editable: isEditable,
        readonly: isReadonly
      });
    });

    // {
    //   headers: ["Column 1", "Column 2", "Column 3", "Column 4", "Column 5"],
    //   rows: [
    //     {
    //       data: ["Data 1", "Data 2", "Data 3", "Data 4", "Data 5"],
    //       editable: false,
    //       readonly: true
    //     },
    //     // more rows...
    //   ]
    // }
    
    // save to local storage
    localStorage.setItem('dynamicTableData', JSON.stringify(tableData));

  }
  
  function loadTableFromLocalStorage() {
    const savedData = localStorage.getItem('dynamicTableData');
    
    if (savedData) {
      const tableData = JSON.parse(savedData);
      
      // update headers
      if (tableData.headers && tableData.headers.length > 0) {
        $('#dynamicTable thead th').each(function(index) {
          // skip first and last columns
          if (index !== 0 && index !== $('#dynamicTable thead th').length - 1) {
            // adjust for the index offset
            const headerIndex = index - 1;
            if (headerIndex < tableData.headers.length) {
              $(this).text(tableData.headers[headerIndex]);
            }
          }
        });
      }
      
      // replace existing with saved data
      if (tableData.rows && tableData.rows.length > 0) {
        $('#dynamicTable tbody').empty();
        
        tableData.rows.forEach(function(rowData) {
          const newRow = $('<tr>' +
            '<td><button class="delete-btn action-button">Delete</button></td>' +
            '<td contenteditable="true"></td>' +
            '<td contenteditable="true"></td>' +
            '<td contenteditable="true"></td>' +
            '<td contenteditable="true"></td>' +
            '<td contenteditable="true"></td>' +
            '<td><button class="add-btn action-button">Add</button></td>' +
            '</tr>');
          
          // set row data
          rowData.data.forEach(function(cellData, index) {
            newRow.find('td:not(:first-child):not(:last-child)').eq(index).text(cellData);
          });
          
          // Set row classes
          if (rowData.readonly) {
            newRow.addClass('readonly');
            newRow.find('td:not(:first-child):not(:last-child)').attr('contenteditable', 'false');
          } else if (rowData.editable) {
            newRow.addClass('editable');
          }
          
          $('#dynamicTable tbody').append(newRow);
        });
      }
    }
  }

  $(window).on('beforeunload', function() {
    saveTableToLocalStorage();
  });
});