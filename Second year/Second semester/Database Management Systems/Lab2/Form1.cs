using Microsoft.Data.SqlClient;
using System.Configuration;
using System.Data;

namespace Lab2
{
    public partial class Form1 : Form
    {
        // Connection string for database connection
        private readonly SqlConnection sqlConnection = new(ConfigurationManager.ConnectionStrings["connectionString"].ConnectionString);

        // UI Components
        private DataGridView parentTable = new();
        private DataGridView childTable = new();
        private Button btnConnect = new();
        private Button btnAdd = new();
        private Button btnUpdate = new();
        private Button btnDelete = new();
        private GroupBox inputGroupBox = new();
        private Panel inputPanel = new();

        // Configuration properties
        private string childTable_name;
        private string childPrimaryKey;
        private string childForeignKey;
        private int childNoOfColumns;
        private string[] childColumns;
        private string parentTable_name;
        private string parentPrimaryKey;

        // Dictionary to store TextBoxes dynamically created
        private Dictionary<string, TextBox> dynamicTextBoxes = new();

        public Form1()
        {
            SetStyle(ControlStyles.DoubleBuffer | ControlStyles.OptimizedDoubleBuffer, true);

            // Load configuration from App.config
            LoadAppConfig();

            InitializeComponent();

            parentTable.SelectionChanged += ParentTable_SelectionChanged;
            childTable.SelectionChanged += ChildTable_SelectionChanged;
        }

        private void LoadAppConfig()
        {
            // Read configuration from App.config
            parentTable_name = ConfigurationManager.AppSettings["ParentTable"];
            parentPrimaryKey = ConfigurationManager.AppSettings["ParentPrimaryKey"];
            childTable_name = ConfigurationManager.AppSettings["ChildTable"];
            childPrimaryKey = ConfigurationManager.AppSettings["ChildPrimaryKey"];
            childForeignKey = ConfigurationManager.AppSettings["ChildForeignKey"];

            // Get child column configuration
            childNoOfColumns = int.Parse(ConfigurationManager.AppSettings["ChildNoOfColumns"]);
            childColumns = ConfigurationManager.AppSettings["ChildColumns"].Split(',');
        }

        private void ChildTable_SelectionChanged(object? sender, EventArgs e)
        {
            // Populate input fields when a child row is selected
            if (childTable.SelectedRows.Count > 0)
            {
                PopulateInputFields(childTable.SelectedRows[0]);
            }
        }

        // Event handler for parent table row selection
        private void ParentTable_SelectionChanged(object? sender, EventArgs e)
        {
            // Load children for the selected parent
            if (parentTable.SelectedRows.Count > 0)
            {
                DataGridViewRow selectedRow = parentTable.SelectedRows[0];
                int parentId = Convert.ToInt32(selectedRow.Cells[parentPrimaryKey.ToLower()].Value);
                LoadChildrenForParent(parentId);

                // Set the foreign key TextBox to the selected parent ID
                if (dynamicTextBoxes.ContainsKey(childForeignKey))
                    dynamicTextBoxes[childForeignKey].Text = parentId.ToString();

                // Clear other input fields
                ClearInputFields(excludeFK: true);
            }
        }

        // Load children for a specific parent
        private void LoadChildrenForParent(int parentId)
        {
            try
            {
                string query = ConfigurationManager.AppSettings["QuerySelectChildForParent"];

                using (SqlDataAdapter sqlDataAdapter = new SqlDataAdapter(query, sqlConnection))
                {
                    // Add the correct parameter name based on the parent table
                    string paramName = $"@{parentTable_name}Id";
                    sqlDataAdapter.SelectCommand.Parameters.AddWithValue(paramName, parentId);

                    DataTable dataTable = new DataTable();
                    sqlDataAdapter.Fill(dataTable);
                    childTable.DataSource = dataTable;

                    // Automatically select the first row if any rows are present
                    if (dataTable.Rows.Count > 0)
                    {
                        // First clear selection to ensure the event fires
                        childTable.ClearSelection();

                        // Manually populate the fields from the first row in the table
                        PopulateInputFields(childTable.Rows[0]);
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, "Child Load Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Extract the population logic to a reusable method
        private void PopulateInputFields(DataGridViewRow row)
        {
            if (row != null)
            {
                // Set the primary key value
                if (dynamicTextBoxes.ContainsKey(childPrimaryKey))
                    dynamicTextBoxes[childPrimaryKey].Text = row.Cells[childPrimaryKey.ToLower()].Value.ToString();

                // Set the foreign key value
                if (dynamicTextBoxes.ContainsKey(childForeignKey))
                    dynamicTextBoxes[childForeignKey].Text = row.Cells[childForeignKey.ToLower()].Value.ToString();

                // Set values for all child columns
                foreach (string column in childColumns)
                {
                    if (dynamicTextBoxes.ContainsKey(column))
                        dynamicTextBoxes[column].Text = row.Cells[column.ToLower()].Value.ToString();
                }
            }
        }

        // Check if a parent exists in the database
        private bool ParentExists(int parentId)
        {
            try
            {
                string query = ConfigurationManager.AppSettings["QueryCheckParentExists"];
                using (SqlCommand cmd = new SqlCommand(query, sqlConnection))
                {
                    // Add the correct parameter name based on the parent table
                    string paramName = $"@{parentTable_name}Id";
                    cmd.Parameters.AddWithValue(paramName, parentId);
                    int count = Convert.ToInt32(cmd.ExecuteScalar());
                    return count > 0;
                }
            }
            catch (SqlException)
            {
                return false;
            }
        }

        // Add new child
        private void BtnAdd_Click(object sender, EventArgs e)
        {
            // Validate input
            if (!ValidateInput(isUpdate: false))
            {
                return;
            }

            // Check if parent exists before adding child
            if (!ParentExists(int.Parse(dynamicTextBoxes[childForeignKey].Text)))
            {
                MessageBox.Show("Parent ID does not exist!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            try
            {
                string query = ConfigurationManager.AppSettings["QueryInsertChild"];

                using (SqlCommand cmd = new SqlCommand(query, sqlConnection))
                {
                    // Add parameters for the foreign key (all lowercase to match config)
                    cmd.Parameters.AddWithValue($"@{childForeignKey.ToLower()}", int.Parse(dynamicTextBoxes[childForeignKey].Text));

                    // Add parameters for each column (all lowercase to match config)
                    foreach (string column in childColumns)
                    {
                        // Handle different data types based on column name
                        if (column.ToLower() == "content")
                            cmd.Parameters.AddWithValue($"@{column.ToLower()}", dynamicTextBoxes[column].Text);
                        else
                            cmd.Parameters.AddWithValue($"@{column.ToLower()}", int.Parse(dynamicTextBoxes[column].Text));
                    }

                    cmd.ExecuteNonQuery();
                }

                // Refresh the DataGridViews
                RefreshDataGridView();

                // Clear input fields
                ClearInputFields();

                MessageBox.Show("Child added successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, "Database Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Update existing child
        private void BtnUpdate_Click(object sender, EventArgs e)
        {
            // Validate input
            if (!ValidateInput(isUpdate: true))
            {
                return;
            }

            // Check if parent exists
            if (!ParentExists(int.Parse(dynamicTextBoxes[childForeignKey].Text)))
            {
                MessageBox.Show("Parent ID does not exist!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            try
            {
                string query = ConfigurationManager.AppSettings["QueryUpdateChild"];

                using (SqlCommand cmd = new SqlCommand(query, sqlConnection))
                {
                    // Add parameters for primary key and foreign key (casing matches SQL query from config)
                    cmd.Parameters.AddWithValue($"@{childPrimaryKey}", int.Parse(dynamicTextBoxes[childPrimaryKey].Text));
                    cmd.Parameters.AddWithValue($"@{childForeignKey}", int.Parse(dynamicTextBoxes[childForeignKey].Text));

                    // Add parameters for each column
                    foreach (string column in childColumns)
                    {
                        // Handle different data types based on column name
                        if (column.ToLower() == "content")
                            cmd.Parameters.AddWithValue($"@{column}", dynamicTextBoxes[column].Text);
                        else
                            cmd.Parameters.AddWithValue($"@{column}", int.Parse(dynamicTextBoxes[column].Text));
                    }

                    cmd.ExecuteNonQuery();
                }

                // Refresh the DataGridViews
                RefreshDataGridView();

                // Clear input fields
                ClearInputFields();

                MessageBox.Show("Child updated successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, "Database Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Delete child
        private void BtnDelete_Click(object sender, EventArgs e)
        {
            // Validate input
            if (string.IsNullOrWhiteSpace(dynamicTextBoxes[childPrimaryKey].Text))
            {
                MessageBox.Show("Please select a child to delete!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Confirm deletion
            var result = MessageBox.Show("Are you sure you want to delete this child?", "Confirm Deletion", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (result == DialogResult.No)
                return;

            try
            {
                string query = ConfigurationManager.AppSettings["QueryDeleteChild"];

                using (SqlCommand cmd = new SqlCommand(query, sqlConnection))
                {
                    cmd.Parameters.AddWithValue($"@{childPrimaryKey}", int.Parse(dynamicTextBoxes[childPrimaryKey].Text));
                    cmd.ExecuteNonQuery();
                }

                // Refresh the DataGridViews
                RefreshDataGridView();

                // Clear input fields
                ClearInputFields();

                MessageBox.Show("Child deleted successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, "Database Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Connect button click handler
        private void BtnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                sqlConnection.Open();

                // Load Parent table
                string parentsQuery = ConfigurationManager.AppSettings["QuerySelectParent"];
                SqlDataAdapter parentsAdapter = new(parentsQuery, sqlConnection);
                DataTable parentsDataTable = new();
                parentsAdapter.Fill(parentsDataTable);
                parentTable.DataSource = parentsDataTable;

                // Load Child table
                string childQuery = ConfigurationManager.AppSettings["QuerySelectChild"];
                SqlDataAdapter childAdapter = new(childQuery, sqlConnection);
                DataTable childDataTable = new();
                childAdapter.Fill(childDataTable);
                childTable.DataSource = childDataTable;

                MessageBox.Show("Database Connected Successfully!", "Connection", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void RefreshDataGridView()
        {
            try
            {
                // Get the currently selected parent row ID
                int? selectedParentId = null;
                if (parentTable.SelectedRows.Count > 0)
                {
                    selectedParentId = Convert.ToInt32(parentTable.SelectedRows[0].Cells[parentPrimaryKey.ToLower()].Value);
                }

                // Refresh Parents table
                string parentsQuery = ConfigurationManager.AppSettings["QuerySelectParent"];
                using (SqlDataAdapter parentsAdapter = new SqlDataAdapter(parentsQuery, sqlConnection))
                {
                    DataTable parentsDataTable = new DataTable();
                    parentsAdapter.Fill(parentsDataTable);
                    parentTable.DataSource = parentsDataTable;
                }

                // Refresh Child table
                if (selectedParentId.HasValue)
                {
                    LoadChildrenForParent(selectedParentId.Value);
                }
                else
                {
                    string childQuery = ConfigurationManager.AppSettings["QuerySelectChild"];
                    using (SqlDataAdapter childAdapter = new SqlDataAdapter(childQuery, sqlConnection))
                    {
                        DataTable childDataTable = new DataTable();
                        childAdapter.Fill(childDataTable);
                        childTable.DataSource = childDataTable;
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, "Refresh Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Clear input fields
        private void ClearInputFields(bool excludeFK = false)
        {
            foreach (var textBox in dynamicTextBoxes)
            {
                // Skip clearing the foreign key if excludeFK is true
                if (excludeFK && textBox.Key == childForeignKey)
                    continue;

                textBox.Value.Clear();
            }
        }

        // Validate user input
        private bool ValidateInput(bool isUpdate)
        {
            // For update, require the primary key
            if (isUpdate && (string.IsNullOrWhiteSpace(dynamicTextBoxes[childPrimaryKey].Text) ||
                !int.TryParse(dynamicTextBoxes[childPrimaryKey].Text, out _)))
            {
                MessageBox.Show("Please select a child record to update!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            // Always require the foreign key
            if (string.IsNullOrWhiteSpace(dynamicTextBoxes[childForeignKey].Text) ||
                !int.TryParse(dynamicTextBoxes[childForeignKey].Text, out _))
            {
                MessageBox.Show("Parent ID is required and must be a number!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            // Validate all column fields
            foreach (string column in childColumns)
            {
                if (string.IsNullOrWhiteSpace(dynamicTextBoxes[column].Text))
                {
                    MessageBox.Show($"Field {column} is required!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return false;
                }

                // Only validate as number if not 'content' field
                if (column.ToLower() != "content" && !int.TryParse(dynamicTextBoxes[column].Text, out _))
                {
                    MessageBox.Show($"Field {column} must be a number!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return false;
                }
            }

            return true;
        }
    }
}