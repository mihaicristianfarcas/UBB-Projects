$(document).ready(function() {

    // delete button click
    $(document).on('click', '.delete-btn', function() {
        $(this).closest('tr').remove();
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
    });

    // cell editing
    $(document).on('input', '[contenteditable="true"]', function() {
        const row = $(this).closest('tr');
        const cells = row.find('td:not(:first-child):not(:last-child)');
        let allFilled = true;

        cells.each(function() {
            if (!$(this).text().trim()) {
                allFilled = false;
                return false;
            }
        });

        if (allFilled) {
            row.removeClass('editable').addClass('readonly');
            cells.attr('contenteditable', 'false');
        }
    });
});