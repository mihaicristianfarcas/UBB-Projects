import numpy as np
import matplotlib.pyplot as plt


def quadratic_function(x, A):
    return 0.5 * np.dot(x.T, np.dot(A, x))


def gradient_quadratic(x, A):
    return np.dot(A, x)


def plot_surface_contour_gradient(A, points, title):
    x = np.linspace(-5, 5, 100)
    y = np.linspace(-5, 5, 100)
    X, Y = np.meshgrid(x, y)
    Z = np.zeros_like(X)

    for i in range(len(x)):
        for j in range(len(y)):
            Z[i, j] = quadratic_function(np.array([X[i, j], Y[i, j]]), A)

    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.7)
    ax.set_title(title)

    plt.contour(X, Y, Z, 20, colors='gray', linestyles="solid")

    for point in points:
        grad = gradient_quadratic(point, A)
        ax.quiver(point[0], point[1], quadratic_function(point, A), grad[0], grad[1], 0, color='red', length=0.5, normalize=True)

    plt.show()


# Matrix for unique minimum
A_min = np.array([[2, 0],
                  [0, 4]])

points_min = [np.array([3, 2]),
              np.array([-2, 1]),
              np.array([0, -3])]

plot_surface_contour_gradient(A_min, points_min, "Unique Minimum")

# Matrix for unique maximum
A_max = np.array([[2, 0],
                  [0, -4]])

points_max = [np.array([3, 2]),
              np.array([-2, 1]),
              np.array([0, -3])]

plot_surface_contour_gradient(A_max, points_max, "Unique Maximum")

# Matrix for unique saddle point
A_saddle = np.array([[2, 1],
                     [1, -2]])

points_saddle = [np.array([3, 2]),
                 np.array([-2, 1]),
                 np.array([0, -3])]

plot_surface_contour_gradient(A_saddle, points_saddle, "Unique Saddle Point")
