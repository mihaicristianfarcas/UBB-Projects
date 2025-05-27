using System.Data;
using System.Drawing;
using System.Windows.Forms;

namespace DBMS
{
    partial class Form1
    {
        private DataGridView workersTable;
        private DataGridView bonusTable;
        private GroupBox inputGroupBox;
        private TextBox txtId;
        private TextBox txtWid;
        private TextBox txtSum;
        private Button btnConnect;
        private Button btnAdd;
        private Button btnUpdate;
        private Button btnDelete;

        private void InitializeComponent()
        {
            // Set font family fallback
            var defaultFontFamily = SystemFonts.DefaultFont.FontFamily;

            // Form settings
            this.Text = "Worker Management System";
            this.Size = new Size(1200, 800);
            this.StartPosition = FormStartPosition.CenterScreen;

            // Workers Table
            workersTable = new DataGridView();
            workersTable.Name = "workersTable";
            workersTable.Location = new Point(20, 40);
            workersTable.Size = new Size(550, 370);
            workersTable.AllowUserToAddRows = false;
            workersTable.AllowUserToDeleteRows = false;
            workersTable.ReadOnly = true;
            workersTable.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            workersTable.MultiSelect = false;
            workersTable.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            workersTable.Font = new Font(defaultFontFamily, 10);

            // Bonus Table
            bonusTable = new DataGridView();
            bonusTable.Name = "bonusTable";
            bonusTable.Location = new Point(620, 40);
            bonusTable.Size = new Size(550, 370);
            bonusTable.AllowUserToAddRows = false;
            bonusTable.AllowUserToDeleteRows = false;
            bonusTable.ReadOnly = true;
            bonusTable.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            bonusTable.MultiSelect = false;
            bonusTable.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            bonusTable.Font = new Font(defaultFontFamily, 10);

            // Input Group Box
            inputGroupBox = new GroupBox();
            inputGroupBox.Text = "Bonus Details";
            inputGroupBox.Location = new Point(30, 480);
            inputGroupBox.Size = new Size(550, 250);
            inputGroupBox.Font = new Font(defaultFontFamily, 10, FontStyle.Bold);

            // Input Fields
            txtId = new TextBox();
            txtWid = new TextBox();
            txtSum = new TextBox();

            var lblId = new Label() { Text = "ID:", Location = new Point(30, 50), AutoSize = true, Font = new Font(defaultFontFamily, 11) };
            txtId.Location = new Point(150, 45);
            txtId.Size = new Size(350, 30);
            txtId.Font = new Font(defaultFontFamily, 11);
            txtId.ReadOnly = true;

            var lblWid = new Label() { Text = "WID:", Location = new Point(30, 100), AutoSize = true, Font = new Font(defaultFontFamily, 11) };
            txtWid.Location = new Point(150, 95);
            txtWid.Size = new Size(350, 30);
            txtWid.Font = new Font(defaultFontFamily, 11);

            var lblSum = new Label() { Text = "Sum:", Location = new Point(30, 150), AutoSize = true, Font = new Font(defaultFontFamily, 11) };
            txtSum.Location = new Point(150, 145);
            txtSum.Size = new Size(350, 30);
            txtSum.Font = new Font(defaultFontFamily, 11);

            // Buttons
            btnConnect = new Button();
            btnAdd = new Button();
            btnUpdate = new Button();
            btnDelete = new Button();

            btnConnect.Text = "Connect";
            btnConnect.Location = new Point(620, 480);
            btnConnect.Size = new Size(250, 45);
            btnConnect.Font = new Font(defaultFontFamily, 11);
            btnConnect.Click += BtnConnect_Click;

            btnAdd.Text = "Add Bonus";
            btnAdd.Location = new Point(620, 540);
            btnAdd.Size = new Size(250, 45);
            btnAdd.Font = new Font(defaultFontFamily, 11);
            btnAdd.Click += BtnAdd_Click;

            btnUpdate.Text = "Update Bonus";
            btnUpdate.Location = new Point(620, 600);
            btnUpdate.Size = new Size(250, 45);
            btnUpdate.Font = new Font(defaultFontFamily, 11);
            btnUpdate.Click += BtnUpdate_Click;

            btnDelete.Text = "Delete Bonus";
            btnDelete.Location = new Point(620, 660);
            btnDelete.Size = new Size(250, 45);
            btnDelete.Font = new Font(defaultFontFamily, 11);
            btnDelete.Click += BtnDelete_Click;

            // Add controls to input group box
            inputGroupBox.Controls.AddRange(new Control[]
            {
                lblId, txtId,
                lblWid, txtWid,
                lblSum, txtSum
            });

            // Table labels
            var lblWorkersTable = new Label()
            {
                Text = "Workers",
                Location = new Point(30, 10),
                AutoSize = true,
                Font = new Font(defaultFontFamily, 12, FontStyle.Bold)
            };

            var lblBonusTable = new Label()
            {
                Text = "Bonuses",
                Location = new Point(620, 10),
                AutoSize = true,
                Font = new Font(defaultFontFamily, 12, FontStyle.Bold)
            };

            // Add all controls to the form
            this.Controls.AddRange(new Control[]
            {
                workersTable,
                bonusTable,
                lblWorkersTable,
                lblBonusTable,
                inputGroupBox,
                btnConnect,
                btnAdd,
                btnUpdate,
                btnDelete
            });
        }
    }
}

