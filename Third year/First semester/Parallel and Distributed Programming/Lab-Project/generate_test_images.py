#!/usr/bin/env python3
"""
Generate test images with known lines and circles for testing Hough transform implementation.
"""

import cv2
import numpy as np
import os

def create_output_dir():
    """Create test_images directory if it doesn't exist."""
    os.makedirs('test_images', exist_ok=True)

def generate_lines_image():
    """Generate an image with several straight lines."""
    img = np.zeros((600, 800, 3), dtype=np.uint8)
    img.fill(255)  # White background
    
    # Draw several lines at different angles
    cv2.line(img, (100, 100), (700, 100), (0, 0, 0), 3)  # Horizontal
    cv2.line(img, (400, 50), (400, 550), (0, 0, 0), 3)   # Vertical
    cv2.line(img, (100, 200), (700, 500), (0, 0, 0), 3)  # Diagonal
    cv2.line(img, (700, 200), (100, 500), (0, 0, 0), 3)  # Diagonal opposite
    cv2.line(img, (200, 300), (600, 350), (0, 0, 0), 3)  # Slight angle
    
    cv2.imwrite('test_images/lines.png', img)
    print("Generated: test_images/lines.png")

def generate_circles_image():
    """Generate an image with several circles."""
    img = np.zeros((600, 800, 3), dtype=np.uint8)
    img.fill(255)  # White background
    
    # Draw several circles of different sizes
    cv2.circle(img, (200, 200), 80, (0, 0, 0), 3)
    cv2.circle(img, (600, 200), 60, (0, 0, 0), 3)
    cv2.circle(img, (400, 400), 100, (0, 0, 0), 3)
    cv2.circle(img, (150, 450), 40, (0, 0, 0), 3)
    cv2.circle(img, (650, 450), 50, (0, 0, 0), 3)
    
    cv2.imwrite('test_images/circles.png', img)
    print("Generated: test_images/circles.png")

def generate_mixed_image():
    """Generate an image with both lines and circles."""
    img = np.zeros((600, 800, 3), dtype=np.uint8)
    img.fill(255)  # White background
    
    # Draw lines
    cv2.line(img, (50, 300), (750, 300), (0, 0, 0), 2)
    cv2.line(img, (400, 50), (400, 550), (0, 0, 0), 2)
    
    # Draw circles
    cv2.circle(img, (200, 150), 60, (0, 0, 0), 2)
    cv2.circle(img, (600, 150), 70, (0, 0, 0), 2)
    cv2.circle(img, (200, 450), 50, (0, 0, 0), 2)
    cv2.circle(img, (600, 450), 80, (0, 0, 0), 2)
    
    cv2.imwrite('test_images/mixed.png', img)
    print("Generated: test_images/mixed.png")

def generate_complex_image():
    """Generate a more complex image with multiple shapes and noise."""
    img = np.zeros((800, 800, 3), dtype=np.uint8)
    img.fill(255)  # White background
    
    # Draw a grid of lines
    for i in range(100, 700, 100):
        cv2.line(img, (100, i), (700, i), (0, 0, 0), 2)
        cv2.line(img, (i, 100), (i, 700), (0, 0, 0), 2)
    
    # Draw some circles
    cv2.circle(img, (250, 250), 80, (0, 0, 0), 3)
    cv2.circle(img, (550, 250), 60, (0, 0, 0), 3)
    cv2.circle(img, (400, 550), 100, (0, 0, 0), 3)
    
    # Add some noise
    noise = np.random.randint(0, 50, (800, 800, 3), dtype=np.uint8)
    img = cv2.subtract(img, noise)
    
    cv2.imwrite('test_images/complex.png', img)
    print("Generated: test_images/complex.png")

def generate_diagonal_lines():
    """Generate an image with diagonal lines for testing edge cases."""
    img = np.zeros((600, 600, 3), dtype=np.uint8)
    img.fill(255)  # White background
    
    # Draw diagonal lines at various angles
    for angle in range(0, 180, 15):
        rad = np.radians(angle)
        length = 400
        cx, cy = 300, 300
        x1 = int(cx - length/2 * np.cos(rad))
        y1 = int(cy - length/2 * np.sin(rad))
        x2 = int(cx + length/2 * np.cos(rad))
        y2 = int(cy + length/2 * np.sin(rad))
        cv2.line(img, (x1, y1), (x2, y2), (0, 0, 0), 2)
    
    cv2.imwrite('test_images/diagonal_lines.png', img)
    print("Generated: test_images/diagonal_lines.png")

def generate_concentric_circles():
    """Generate an image with concentric circles."""
    img = np.zeros((600, 600, 3), dtype=np.uint8)
    img.fill(255)  # White background
    
    cx, cy = 300, 300
    for radius in range(50, 250, 40):
        cv2.circle(img, (cx, cy), radius, (0, 0, 0), 2)
    
    cv2.imwrite('test_images/concentric_circles.png', img)
    print("Generated: test_images/concentric_circles.png")

def generate_real_world_example():
    """Generate a simple real-world-like scene."""
    img = np.zeros((600, 800, 3), dtype=np.uint8)
    img.fill(240)  # Light gray background
    
    # Draw a simple house
    # Walls
    cv2.rectangle(img, (200, 300), (600, 550), (100, 100, 100), 2)
    # Roof
    pts = np.array([[200, 300], [400, 150], [600, 300]], np.int32)
    cv2.polylines(img, [pts], True, (100, 100, 100), 2)
    # Door
    cv2.rectangle(img, (350, 450), (450, 550), (50, 50, 50), 2)
    # Windows (circles)
    cv2.circle(img, (300, 400), 40, (50, 50, 50), 2)
    cv2.circle(img, (500, 400), 40, (50, 50, 50), 2)
    
    # Ground line
    cv2.line(img, (0, 550), (800, 550), (100, 100, 100), 2)
    
    # Add slight texture
    noise = np.random.randint(0, 20, (600, 800, 3), dtype=np.uint8)
    img = cv2.subtract(img, noise)
    
    cv2.imwrite('test_images/house.png', img)
    print("Generated: test_images/house.png")

def main():
    print("Generating test images for Hough transform...")
    print("=" * 50)
    
    create_output_dir()
    
    generate_lines_image()
    generate_circles_image()
    generate_mixed_image()
    generate_complex_image()
    generate_diagonal_lines()
    generate_concentric_circles()
    generate_real_world_example()
    
    print("=" * 50)
    print("All test images generated successfully!")
    print("\nTest images:")
    print("  - lines.png: Simple straight lines")
    print("  - circles.png: Simple circles")
    print("  - mixed.png: Both lines and circles")
    print("  - complex.png: Complex scene with grid and circles")
    print("  - diagonal_lines.png: Lines at various angles")
    print("  - concentric_circles.png: Concentric circles")
    print("  - house.png: Real-world-like scene")

if __name__ == "__main__":
    main()

