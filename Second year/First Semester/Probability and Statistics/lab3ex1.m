clear all
clc
pause(1); % to keep first output

% alpha=input("alpha=");
% beta=input("beta=");
alpha=0.2;
beta=0.2;
option=input("Enter the distribution model (Normal, Student, Chi2, Fisher)=",'s');

switch option
    case 'Normal'
        fprintf('Normal distribution model\n');
        % mu=input("mu=");
        % sigma=input("sigma=");
        mu=1;
        sigma=0.5;
        fprintf("a)\nP(x<=0)=%4f\nP(x>=0)=%4f\n", normcdf(0, mu, sigma), 1 - normcdf(0, mu, sigma));
        fprintf("b)\nP(-1<=x<=1)=%4f\nP(x<=-1 or x>=1)=%4f\n", normcdf(1, mu, sigma) - normcdf(-1, mu, sigma), 1 - (normcdf(1, mu, sigma) - normcdf(-1, mu, sigma)));
        fprintf("c)\nP(x<Xa)=P(x<=Xa)=%4f\n", norminv(alpha, mu, sigma));
        fprintf("d)\nP(x>Xa)=P(x>=Xa)=%4f\n", norminv(1 - beta, mu, sigma));
    case 'Student'
        fprintf('Student distribution model\n');
        n=input("n=");
        fprintf("a)\nP(x<=0)=%4f\nP(x>=0)=%4f\n", tcdf(0, n), 1 - tcdf(0, n));
        fprintf("b)\nP(-1<=x<=1)=%4f\nP(x<=-1 or x>=1)=%4f\n", tcdf(1, n) - tcdf(-1, n), 1 - (tcdf(1, n) - tcdf(-1, n)));
        fprintf("c)\nP(x<Xa)=P(x<=Xa)=%4f\n", tinv(alpha, n));
        fprintf("d)\nP(x>Xa)=P(x>=Xa)=%4f\n", tinv(1 - beta, n));
    case 'Chi2'
        fprintf('Chi2 distribution model\n');
        n=input("n=");
        fprintf("a)\nP(x<=0)=%4f\nP(x>=0)=%4f\n", chi2cdf(0, n), 1 - chi2cdf(0, n));
        fprintf("b)\nP(-1<=x<=1)=%4f\nP(x<=-1 or x>=1)=%4f\n", chi2cdf(1, n) - chi2cdf(-1, n), 1 - (chi2cdf(1, n) - chi2cdf(-1, n)));
        fprintf("c)\nP(x<Xa)=P(x<=Xa)=%4f\n", chi2inv(alpha, n));
        fprintf("d)\nP(x>Xa)=P(x>=Xa)=%4f\n", chi2inv(1 - beta, n));
    case 'Fisher'
        fprintf('Fisher distribution model\n');
        m=input("m=");
        n=input("n=");
        fprintf("a)\nP(x<=0)=%4f\nP(x>=0)=%4f\n", fcdf(0, m, n), 1 - fcdf(0, m, n));
        fprintf("b)\nP(-1<=x<=1)=%4f\nP(x<=-1 or x>=1)=%4f\n", fcdf(1, m, n) - fcdf(-1, m, n), 1 - (fcdf(1, m, n) - fcdf(-1, m, n)));
        fprintf("c)\nP(x<Xa)=P(x<=Xa)=%4f\n", finv(alpha, m, n));
        fprintf("d)\nP(x>Xa)=P(x>=Xa)=%4f\n", finv(1 - beta, m, n));
    otherwise
        fprintf("Wrong option.\n");
end