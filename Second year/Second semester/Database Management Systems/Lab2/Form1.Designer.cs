// Modified Form1.Designer.cs
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using System.Configuration;

namespace Lab2
{
    partial class Form1
    {
        private void InitializeComponent()
        {
            // Form settings
            this.Text = "Management System";
            this.Size = new Size(1200, 800);
            this.StartPosition = FormStartPosition.CenterScreen;

            // Get table names from configuration
            string parentTableName = ConfigurationManager.AppSettings["ParentTable"];
            string childTableName = ConfigurationManager.AppSettings["ChildTable"];

            // Parent Table
            parentTable.Name = "parentTable";
            parentTable.Location = new Point(20, 40);
            parentTable.Size = new Size(550, 370);
            parentTable.AllowUserToAddRows = false;
            parentTable.AllowUserToDeleteRows = false;
            parentTable.ReadOnly = true;
            parentTable.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            parentTable.MultiSelect = false;
            parentTable.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            parentTable.Font = new Font(Font.FontFamily, 10);

            // Child Table
            childTable.Name = "childTable";
            childTable.Location = new Point(620, 40);
            childTable.Size = new Size(550, 370);
            childTable.AllowUserToAddRows = false;
            childTable.AllowUserToDeleteRows = false;
            childTable.ReadOnly = true;
            childTable.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            childTable.MultiSelect = false;
            childTable.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            childTable.Font = new Font(Font.FontFamily, 10);

            // Input Group Box
            inputGroupBox.Text = $"{childTableName} Details";
            inputGroupBox.Location = new Point(30, 480);
            inputGroupBox.Size = new Size(550, 250);
            inputGroupBox.Font = new Font(Font.FontFamily, 10, FontStyle.Bold);

            // Input Panel for dynamic TextBoxes
            inputPanel.Location = new Point(20, 30);
            inputPanel.Size = new Size(510, 200);
            inputPanel.AutoScroll = true;

            // Create dynamic TextBoxes based on configuration
            CreateDynamicInputs();

            // Buttons
            btnConnect.Text = "Connect";
            btnConnect.Location = new Point(620, 480);
            btnConnect.Size = new Size(250, 45);
            btnConnect.Font = new Font(Font.FontFamily, 11);
            btnConnect.Click += BtnConnect_Click;

            btnAdd.Text = $"Add {childTableName}";
            btnAdd.Location = new Point(620, 540);
            btnAdd.Size = new Size(250, 45);
            btnAdd.Font = new Font(Font.FontFamily, 11);
            btnAdd.Click += BtnAdd_Click;

            btnUpdate.Text = $"Update {childTableName}";
            btnUpdate.Location = new Point(620, 600);
            btnUpdate.Size = new Size(250, 45);
            btnUpdate.Font = new Font(Font.FontFamily, 11);
            btnUpdate.Click += BtnUpdate_Click;

            btnDelete.Text = $"Delete {childTableName}";
            btnDelete.Location = new Point(620, 660);
            btnDelete.Size = new Size(250, 45);
            btnDelete.Font = new Font(Font.FontFamily, 11);
            btnDelete.Click += BtnDelete_Click;

            // Add inputPanel to group box
            inputGroupBox.Controls.Add(inputPanel);

            // Add all controls to form
            this.Controls.AddRange(new Control[] {
                parentTable,
                childTable,
                inputGroupBox,
                btnConnect,
                btnAdd,
                btnUpdate,
                btnDelete
            });

            // Labels for tables
            Label lblParentsTable = new Label()
            {
                Text = parentTableName,
                Location = new Point(30, 0),
                AutoSize = true,
                Font = new Font(Font.FontFamily, 12, FontStyle.Bold)
            };

            Label lblChildrenTable = new Label()
            {
                Text = childTableName,
                Location = new Point(620, 0),
                AutoSize = true,
                Font = new Font(Font.FontFamily, 12, FontStyle.Bold)
            };

            this.Controls.Add(lblParentsTable);
            this.Controls.Add(lblChildrenTable);
        }

        private void CreateDynamicInputs()
        {
            int yPos = 10;
            const int labelX = 10;
            const int textBoxX = 120;
            const int controlHeight = 40;
            const int textBoxWidth = 350;

            // Get configuration
            string childPrimaryKey = ConfigurationManager.AppSettings["ChildPrimaryKey"];
            string childForeignKey = ConfigurationManager.AppSettings["ChildForeignKey"];
            string[] childColumns = ConfigurationManager.AppSettings["ChildColumns"].Split(',');

            // Create input for primary key (read-only)
            CreateLabelAndTextBox(childPrimaryKey, yPos, labelX, textBoxX, textBoxWidth, true);
            yPos += controlHeight;

            // Create input for foreign key
            CreateLabelAndTextBox(childForeignKey, yPos, labelX, textBoxX, textBoxWidth);
            yPos += controlHeight;

            // Create inputs for all columns
            foreach (string column in childColumns)
            {
                CreateLabelAndTextBox(column, yPos, labelX, textBoxX, textBoxWidth);
                yPos += controlHeight;
            }
        }

        private void CreateLabelAndTextBox(string fieldName, int yPos, int labelX, int textBoxX, int textBoxWidth, bool readOnly = false)
        {
            Label label = new Label()
            {
                Text = $"{fieldName}:",
                Location = new Point(labelX, yPos + 5),
                AutoSize = true,
                Font = new Font(Font.FontFamily, 11)
            };

            TextBox textBox = new TextBox()
            {
                Name = $"txt{fieldName}",
                Location = new Point(textBoxX + 20, yPos),
                Size = new Size(textBoxWidth, 30),
                Font = new Font(Font.FontFamily, 11),
                ReadOnly = readOnly
            };

            inputPanel.Controls.Add(label);
            inputPanel.Controls.Add(textBox);

            // Store TextBox in dictionary for later access
            dynamicTextBoxes[fieldName] = textBox;
        }
    }
}
