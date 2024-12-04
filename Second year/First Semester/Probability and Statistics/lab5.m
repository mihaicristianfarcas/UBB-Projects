clear
clc
pause(1);

% part 1
%{

% freq. vector
X = [20, 20, 21, 22, 22, 22, 23 * ones(1, 6), 24 * ones(1, 5), 25 * ones(1, 9), 26, 26, 27, 27];
Y = [75, 75, 75, 76, 76, 77, 77, 78 * ones(1, 5), 79 * ones(1, 8), 80 * ones(1, 8), 81, 82];

% arith. mean
mean(X)
mean(Y)

% variation
variationX = var(X, 1)
variationY = var(Y, 1)

% standard dev. std(X) when we don't already have var(X)
sqrt(variationX)
sqrt(variationY)
%}

% part 2
% 1.

X = [7, 7, 4, 5, 9, 9, 4, 12, 8, 1, 8, 7, 3, 13, 2, 1, 17, 7, 12, 5, 6, 2, 1, 13, 14, 10, 2, 4, 9, 11, 3, 5, 12, 6, 10, 70];
N = length(X);

conflvl = input("Confidence level");
a = 1 - conflvl;

% a.
%{
z1 = norminv(1 - a / 2, 0, 1);
z2 = norminv(a / 2, 0, 1);

mX = mean(X)
fprintf("%f\n", mX - (5 / sqrt(length(X))) * z1)
fprintf("%f\n", mX - (5 / sqrt(length (X))) * z2)
%}

% b.
%{
z1 = tinv(1 - a / 2, N - 1);
z2 = tinv(a / 2, N - 1);
mX = std(X);

fprintf("%f\n", mX - (5 / sqrt(length(X))) * z1)
fprintf("%f\n", mX - (5 / sqrt(length (X))) * z2)
%}

% c.
%{
s2 = var(X);
z1 = chi2inv(1 - a / 2, N - 1);
z2 = chi2inv(a / 2, N - 1);

fprintf("(%f, %f)\n", sqrt((N - 1) * s2 / z1), sqrt((N - 1) * s2 / z2));
%}

% 2. -> model 3 from conf. inv.
% a.

Premium = [22.4, 21.7, 24.5, 23.4, 21.6, 23.3, 22.4, 21.6, 24.8, 20.0];
Regular = [17.7, 14.8, 19.6, 19.6, 12.1, 14.8, 15.4, 12.6, 14.0, 12.2];

lenP = length(Premium);
lenR = length(Regular);

meanP = mean(Premium);
meanR = mean(Regular);

len = lenP + lenR - 2;

z = tinv(1 - a / 2, len);

sP = var(Premium);
sR = var(Regular);

s = sqrt(((lenP - 1) * sP + (lenR - 1) * sR) / len);

fprintf("%f\n", meanP - meanR - z * s * sqrt(1 / lenP + 1 / lenR));
fprintf("%f\n", meanP - meanR + z * s * sqrt(1 / lenP + 1 / lenR));

% b. and c. hw