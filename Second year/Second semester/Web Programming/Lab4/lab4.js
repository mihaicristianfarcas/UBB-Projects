class SimpleCombobox {
  constructor(inputField, toggleButton, listbox) {
    this.inputField = inputField;
    this.toggleButton = toggleButton;
    this.listbox = listbox;

    this.allOptions = Array.from(listbox.getElementsByTagName("LI"));
    this.filteredOptions = [];

    this.inputField.addEventListener("input", this.handleInput.bind(this));
    this.toggleButton.addEventListener("click", this.toggleDropdown.bind(this));

    this.allOptions.forEach((option) => {
      option.addEventListener("click", this.selectOption.bind(this, option));
    });

    document.addEventListener("click", this.handleOutsideClick.bind(this));

    this.closeDropdown();
  }

  handleInput() {
    const inputText = this.inputField.value.toLowerCase();

    // Clear and repopulate the filtered options
    this.filteredOptions = [];
    this.listbox.innerHTML = "";

    // Filter
    this.allOptions.forEach((option) => {
      const optionText = option.textContent.toLowerCase();
      if (optionText.startsWith(inputText)) {
        this.filteredOptions.push(option);
        this.listbox.appendChild(option);
      }
    });

    if (this.filteredOptions.length > 0 && inputText.length > 0) {
      this.openDropdown();
    } else if (inputText.length === 0) {
      // If input is empty, show all options
      this.allOptions.forEach((option) => this.listbox.appendChild(option));
      this.filteredOptions = [...this.allOptions];
      this.openDropdown();
    } else {
      // No matching options
      this.closeDropdown();
    }
  }

  toggleDropdown() {
    if (this.isDropdownOpen()) {
      this.closeDropdown();
    } else {
      this.openDropdown();
    }
    this.inputField.focus();
  }

  selectOption(option) {
    this.inputField.value = option.textContent;
    this.closeDropdown();
  }

  handleOutsideClick(event) {
    if (
      !this.inputField.contains(event.target) &&
      !this.toggleButton.contains(event.target) &&
      !this.listbox.contains(event.target)
    ) {
      this.closeDropdown();
    }
  }

  isDropdownOpen() {
    return this.listbox.style.display === "block";
  }

  openDropdown() {
    this.listbox.style.display = "block";
  }

  closeDropdown() {
    this.listbox.style.display = "none";
  }
}

// Initialize the combobox when the page loads
window.addEventListener("load", function () {
  const combobox = document.querySelector(".combobox-list");
  const input = combobox.querySelector("input");
  const button = combobox.querySelector("button");
  const listbox = combobox.querySelector('[role="listbox"]');

  new SimpleCombobox(input, button, listbox);
});
