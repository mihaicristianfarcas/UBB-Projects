using Microsoft.Data.SqlClient;
using System.Data;

namespace DBMS
{
    public partial class Form1 : Form
    {
        private readonly SqlConnection sqlConnection = new("Data Source=MIHAICRISTI8E18\\SQLEXPRESS; Initial Catalog=DBMS; Integrated Security=True; TrustServerCertificate=True");

        private DataSet dataSet = new();
        private SqlDataAdapter workersAdapter;
        private SqlDataAdapter bonusAdapter;
        private BindingSource workersBindingSource = new();
        private BindingSource bonusBindingSource = new();

        public Form1()
        {
            InitializeComponent();
            workersTable.SelectionChanged += WorkersTable_SelectionChanged;
            bonusTable.SelectionChanged += BonusTable_SelectionChanged;
        }

        private void BtnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                sqlConnection.Open();

                workersAdapter = new SqlDataAdapter("SELECT * FROM Worker", sqlConnection);
                bonusAdapter = new SqlDataAdapter("SELECT * FROM Bonus", sqlConnection);

                dataSet.Clear(); // clear old data

                workersAdapter.Fill(dataSet, "Worker");
                bonusAdapter.Fill(dataSet, "Bonus");

                workersBindingSource.DataSource = dataSet.Tables["Worker"];
                bonusBindingSource.DataSource = dataSet.Tables["Bonus"];

                workersTable.DataSource = workersBindingSource;
                bonusTable.DataSource = bonusBindingSource;

                MessageBox.Show("Database Connected Successfully!", "Connection", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (SqlException ex)
            {
                MessageBox.Show(ex.Message, "Connection Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void WorkersTable_SelectionChanged(object? sender, EventArgs e)
        {
            if (workersTable.SelectedRows.Count > 0)
            {
                int workerId = Convert.ToInt32(((DataRowView)workersBindingSource.Current)["id"]);
                LoadBonusesForWorker(workerId);
            }
        }

        private void LoadBonusesForWorker(int workerId)
        {
            bonusBindingSource.Filter = $"wid = {workerId}";
            if (bonusBindingSource.Count == 1)
            {
                bonusTable.Rows[0].Selected = true;
            }
        }

        private void BonusTable_SelectionChanged(object? sender, EventArgs e)
        {
            if (bonusTable.SelectedRows.Count > 0)
            {
                var row = (DataRowView)bonusBindingSource.Current;
                txtId.Text = row["id"].ToString();
                txtWid.Text = row["wid"].ToString();
                txtSum.Text = row["sum"].ToString();
            }
        }

        private bool WorkerExists(int workerId)
        {
            return dataSet.Tables["Worker"].Select($"id = {workerId}").Length > 0;
        }

        private void BtnAdd_Click(object sender, EventArgs e)
        {
            if (!int.TryParse(txtWid.Text, out int wid) || !int.TryParse(txtSum.Text, out int sum))
            {
                MessageBox.Show("Please fill all fields correctly!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (!WorkerExists(wid))
            {
                MessageBox.Show("Worker ID does not exist!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            using SqlCommand insertCommand = new("INSERT INTO Bonus (wid, sum) VALUES (@wid, @sum)", sqlConnection);
            insertCommand.Parameters.AddWithValue("@wid", wid);
            insertCommand.Parameters.AddWithValue("@sum", sum);
            insertCommand.ExecuteNonQuery();

            RefreshBonusData();
            ClearInputFields();
            MessageBox.Show("Bonus added successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void BtnUpdate_Click(object sender, EventArgs e)
        {
            if (!int.TryParse(txtId.Text, out int id) ||
                !int.TryParse(txtWid.Text, out int wid) ||
                !int.TryParse(txtSum.Text, out int sum))
            {
                MessageBox.Show("Please select a bonus and fill all fields correctly!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (!WorkerExists(wid))
            {
                MessageBox.Show("Worker ID does not exist!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            using SqlCommand updateCommand = new("UPDATE Bonus SET wid = @wid, sum = @sum WHERE id = @id", sqlConnection);
            updateCommand.Parameters.AddWithValue("@id", id);
            updateCommand.Parameters.AddWithValue("@wid", wid);
            updateCommand.Parameters.AddWithValue("@sum", sum);
            updateCommand.ExecuteNonQuery();

            RefreshBonusData();
            ClearInputFields();
            MessageBox.Show("Bonus updated successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void BtnDelete_Click(object sender, EventArgs e)
        {
            if (!int.TryParse(txtId.Text, out int id))
            {
                MessageBox.Show("Please select a bonus to delete!", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (MessageBox.Show("Are you sure you want to delete this bonus?", "Confirm Deletion", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
                return;

            using SqlCommand deleteCommand = new("DELETE FROM Bonus WHERE id = @id", sqlConnection);
            deleteCommand.Parameters.AddWithValue("@id", id);
            deleteCommand.ExecuteNonQuery();

            RefreshBonusData();
            ClearInputFields();
            MessageBox.Show("Bonus deleted successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void RefreshBonusData()
        {
            dataSet.Tables["Bonus"].Clear();
            bonusAdapter.Fill(dataSet, "Bonus");
        }

        private void ClearInputFields()
        {
            txtId.Clear();
            txtWid.Clear();
            txtSum.Clear();
        }
    }
}

