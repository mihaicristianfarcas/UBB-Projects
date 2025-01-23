clear all
clc
pause(1)

% TEST - Problem 7 - 2025
% Farcas Mihai-Cristian - 923/2

A = [1021, 980, 1017, 988, 1005, 998, 1014, 985, 995, 1004, 1030, 1015, 995, 1023];
B = [1070, 970, 993, 1013, 1006, 1002, 1014, 997, 1002, 1010, 975];

% a.
% H0: sigmaA = sigmaB = theta0 null hypothesis, pop. variations coincide
% H1: sigmaA != sigmaB = theta1 alternative hypothesis, pop. variances differ

% tail values: -1 for left tail testing
%            : 0 for two tail testing, default
%            : 1 for right tail testing

% 5% significance level -> alpha = 0.05
% We use the F (Fisher) distribution model
% using vartest2 to test variation difference

tail = 0;
alpha = 0.05;
[h, p, ci, stats] = vartest2(A, B, alpha, tail);

% h - 0/1 hypothesis not rejected/rejected

% statistical question answer
if h == 1
    fprintf('H0 is rejected\n');
else
    fprintf('H0 is not rejected\n');
end

% real question answer
if h == 1
    fprintf('Population variances are not equal\n');
else
    fprintf('Population variances are equal\n');
end

%{
we can also use the other obtained values (p, observed) to check hypothesis:

if p < alpha % P-value
    fprintf("We reject H0\n");
else
    fprintf("We don't reject H0\n");
end

if stats.fstat < invalpha % TS0 in RR
    fprintf("Population variances are not equal.\n");
else
    fprintf("Population variances are equal.\n");
end
%}

% df - degrees of freedom
dfA = length(A) - 1;
dfB = length(B) - 1;

tt1 = finv(alpha / 2, dfA, dfB);
tt2 = finv(1 - alpha / 2, dfA, dfB);

% Rejection region
fprintf('Rejection region R is (-inf, %4f) U (%4f, inf)\n', tt1, tt2);

% P-value
fprintf('P-value is %4f\n', p);

% Observed value of the statistical test
fprintf('Observed value is %4f\n', stats.fstat);


% b.
% 95% confidence -> 100(1 - alpha)% -> alpha = 0.05
% we don't know variances but we know they are equal
% -> T (Student) distribution model for the difference of the average weights

meanA = mean(A);
meanB = mean(B);

lenA = length(A);
lenB = length(B);
len = dfA + dfB; % length(A) + length(B) - 2

z = tinv(1 - alpha / 2, len);

sA = var(A);
sB = var(B);

s = sqrt((dfA * sA + dfB * sB) / len);

fprintf("\n95 percent confidence interval for the difference of average weights:\n")
fprintf("(%f, ", meanA - meanB - z * s * sqrt(1 / lenA + 1 / lenB));
fprintf("%f)\n", meanA - meanB + z * s * sqrt(1 / lenB + 1 / lenA));
