function addOption() {
  const input = document.getElementById("newOption");
  const comboBox = document.getElementById("editableComboBox");
  const newOptionValue = input.value.trim();

  if (newOptionValue) {
    const newOption = document.createElement("option");
    newOption.value = newOptionValue;
    newOption.textContent = newOptionValue;
    comboBox.appendChild(newOption);
    input.value = ""; // Clear the input field
  } else {
    alert("Please enter a valid option.");
  }
}
