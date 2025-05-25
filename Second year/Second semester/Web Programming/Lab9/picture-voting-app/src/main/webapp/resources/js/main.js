// Wait for the DOM to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
    // Initialize tooltips
    const tooltipTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="tooltip"]'));
    tooltipTriggerList.map(function (tooltipTriggerEl) {
        return new bootstrap.Tooltip(tooltipTriggerEl);
    });

    // Initialize popovers
    const popoverTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="popover"]'));
    popoverTriggerList.map(function (popoverTriggerEl) {
        return new bootstrap.Popover(popoverTriggerEl);
    });

    // Auto-hide alerts after 5 seconds
    const alerts = document.querySelectorAll('.alert');
    alerts.forEach(alert => {
        setTimeout(() => {
            const bsAlert = new bootstrap.Alert(alert);
            bsAlert.close();
        }, 5000);
    });

    // Image preview for upload
    const imageInput = document.getElementById('picture');
    if (imageInput) {
        imageInput.addEventListener('change', function(e) {
            const file = e.target.files[0];
            if (file) {
                const reader = new FileReader();
                const preview = document.getElementById('imagePreview');
                
                reader.onload = function(event) {
                    if (!preview) {
                        const previewHtml = `
                            <div class="mt-2 text-center">
                                <img src="${event.target.result}" class="img-thumbnail" style="max-height: 200px;">
                            </div>`;
                        imageInput.insertAdjacentHTML('afterend', previewHtml);
                    } else {
                        preview.innerHTML = `<img src="${event.target.result}" class="img-thumbnail" style="max-height: 200px;">`;
                    }
                };
                
                reader.readAsDataURL(file);
            }
        });
    }

    // Confirm before deleting
    const deleteForms = document.querySelectorAll('form[onsubmit*="confirm"]');
    deleteForms.forEach(form => {
        form.onsubmit = function() {
            return confirm('Are you sure you want to delete this item? This action cannot be undone.');
        };
    });

    // Initialize file input labels
    const fileInputs = document.querySelectorAll('.form-control[type="file"]');
    fileInputs.forEach(input => {
        input.addEventListener('change', function() {
            const fileName = this.files[0]?.name || 'Choose file';
            const label = this.nextElementSibling;
            if (label && label.classList.contains('custom-file-label')) {
                label.textContent = fileName;
            }
        });
    });
});

// Utility function to show a toast notification
function showToast(message, type = 'success') {
    const toastContainer = document.getElementById('toastContainer') || createToastContainer();
    const toastId = 'toast-' + Date.now();
    const toastHtml = `
        <div id="${toastId}" class="toast align-items-center text-white bg-${type} border-0" role="alert" aria-live="assertive" aria-atomic="true">
            <div class="d-flex">
                <div class="toast-body">
                    ${message}
                </div>
                <button type="button" class="btn-close btn-close-white me-2 m-auto" data-bs-dismiss="toast" aria-label="Close"></button>
            </div>
        </div>
    `;
    
    toastContainer.insertAdjacentHTML('beforeend', toastHtml);
    const toastElement = document.getElementById(toastId);
    const toast = new bootstrap.Toast(toastElement, { autohide: true, delay: 3000 });
    toast.show();
    
    // Remove the toast after it's hidden
    toastElement.addEventListener('hidden.bs.toast', function () {
        toastElement.remove();
    });
}

// Create toast container if it doesn't exist
function createToastContainer() {
    const container = document.createElement('div');
    container.id = 'toastContainer';
    container.style.position = 'fixed';
    container.style.top = '20px';
    container.style.right = '20px';
    container.style.zIndex = '1100';
    document.body.appendChild(container);
    return container;
}

// AJAX form submission helper
function submitForm(form, options = {}) {
    const { method = 'POST', onSuccess, onError } = options;
    
    return new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        const formData = new FormData(form);
        
        xhr.open(method, form.action, true);
        xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest');
        
        xhr.onload = function() {
            if (xhr.status >= 200 && xhr.status < 300) {
                try {
                    const response = JSON.parse(xhr.responseText);
                    if (onSuccess) onSuccess(response);
                    resolve(response);
                } catch (e) {
                    if (onError) onError(e);
                    reject(e);
                }
            } else {
                const error = new Error(xhr.statusText);
                error.status = xhr.status;
                if (onError) onError(error);
                reject(error);
            }
        };
        
        xhr.onerror = function() {
            const error = new Error('Network error');
            if (onError) onError(error);
            reject(error);
        };
        
        xhr.send(formData);
    });
}
