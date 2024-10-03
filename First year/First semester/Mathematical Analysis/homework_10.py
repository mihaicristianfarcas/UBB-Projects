import numpy as np
import matplotlib.pyplot as plt


def quadratic_function(x, y, b):
    return 0.5 * (x ** 2 + b * y ** 2)


def gradient(x, y, b):
    df_dx = x
    df_dy = b * y
    return np.array([df_dx, df_dy])


def gradient_descent(b, learning_rate=0.1, num_iterations=50):
    x, y = 2.0, 2.0

    x_values, y_values = [x], [y]

    for _ in range(num_iterations):
        grad = gradient(x, y, b)
        x -= learning_rate * grad[0]
        y -= learning_rate * grad[1]
        x_values.append(x)
        y_values.append(y)

    return x_values, y_values


b_values = [1, 0.5, 2, 0.2, 0.1]

fig = plt.figure(figsize=(20, 10))

for i, b in enumerate(b_values):
    ax = fig.add_subplot(1, len(b_values), i + 1, projection='3d')

    X, Y = np.meshgrid(np.linspace(-3, 3, 100), np.linspace(-3, 3, 100))
    Z = quadratic_function(X, Y, b)

    ax.plot_surface(X, Y, Z, cmap='plasma', alpha=1)

    x_values, y_values = gradient_descent(b)

    ax.plot(x_values, y_values, quadratic_function(np.array(x_values), np.array(y_values), b), marker='*', linestyle='-', color='black')

    ax.set_title(f'b = {b}')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')

plt.show()