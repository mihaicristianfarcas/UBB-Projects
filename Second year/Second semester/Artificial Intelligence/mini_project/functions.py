import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

def rastrigin(x, y):
    """
    Rastrigin function (highly multimodal with many local minima)
    Domain: [-5.12, 5.12] × [-5.12, 5.12]
    Global minimum: f(0,0) = 0
    """
    return 20 + x**2 + y**2 - 10 * (np.cos(2 * np.pi * x) + np.cos(2 * np.pi * y))

def ackley(x, y):
    """
    Ackley function (multimodal with global minimum at (0,0))
    Domain: [-5, 5] × [-5, 5]
    Global minimum: f(0,0) = 0
    """
    term1 = -20 * np.exp(-0.2 * np.sqrt(0.5 * (x**2 + y**2)))
    term2 = -np.exp(0.5 * (np.cos(2 * np.pi * x) + np.cos(2 * np.pi * y)))
    return term1 + term2 + np.e + 20

def visualize_function(func, domain, func_name, resolution=100):
    """
    Visualize a 2D function with contour and 3D surface plots
    
    Parameters:
    - func: The function to visualize (takes x and y as inputs)
    - domain: List of domain boundaries [x_min, x_max, y_min, y_max]
    - func_name: Name of the function for plot titles
    - resolution: Resolution of the grid (higher = smoother but slower)
    """
    x_min, x_max, y_min, y_max = domain
    
    # Create grid points
    x = np.linspace(x_min, x_max, resolution)
    y = np.linspace(y_min, y_max, resolution)
    X, Y = np.meshgrid(x, y)
    Z = func(X, Y)
    
    # Create figure with two subplots
    fig = plt.figure(figsize=(15, 6))
    
    # 3D Surface plot
    ax1 = fig.add_subplot(1, 2, 1, projection='3d')
    surf = ax1.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=True, alpha=0.8)
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_zlabel('f(X,Y)')
    ax1.set_title(f'3D Surface Plot of {func_name}')
    fig.colorbar(surf, ax=ax1, shrink=0.5, aspect=5)
    
    # Contour plot
    ax2 = fig.add_subplot(1, 2, 2)
    contour = ax2.contourf(X, Y, Z, 50, cmap=cm.coolwarm)
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_title(f'Contour Plot of {func_name}')
    fig.colorbar(contour, ax=ax2)
    
    plt.tight_layout()
    plt.show()