clear all
clc
pause(1)

%{ 
Bernoulli Distribution
S=input("Number of simulations: ");
p=input("Probability of success: ");

U=rand(n, S);
X=U<p;

UX=unique(X);
nX=hist(X, length(UX)); % 'hist' counts nr. of occurences
rel_freq=nX/S
[UX; rel_freq]
%}

%{
% Binomial Distribution
S=input("Number of simulations: ");
n=input("Number of trials: ");
p=input("Probability of success: ");

U=rand(n, S);
X=sum(U<p);

UX=unique(X);
nX=hist(X, length(UX)); % 'hist' counts nr. of occurences
rel_freq=nX/S;

plot(UX, rel_freq, '*');
hold on;
plot(0:n, binopdf(0:n, n, p), '^');
hold off;

title("Binomial Model");
legend("Simulation", "Theoretical");
%}

%{
% Geometrical Distribution
S=input("Number of simulations: ");
p=input("Probability of success: ");

X=zeros(1, S);

for i=1:S
    noFailures=0;
    while rand<=p % while we don't have a failure
        noFailures=noFailures + 1;
    end
    X(i)=noFailures;
end

UX=unique(X);
nX=hist(X, length(UX)); % 'hist' counts nr. of occurences
rel_freq=nX/S;

plot(UX, rel_freq, '*');
hold on;
plot(0:max(UX), geopdf(0:max(UX), p), '^');
hold off;

title("Geometrical Model");
legend("Simulation", "Theoretical")
%}

% Pascal Distribution
S=input("Number of simulations: ");
n=input("Number of successes: ");
p=input("Probability of success: ");

X=zeros(1, S);

for i=1:S
    noFailures=0;
    noSuccesses=0;
    while noSuccesses ~= n % while we don't have a failure
        if rand>=p
            noFailures=noFailures + 1;
        else
            noSuccesses=noSuccesses + 1;
        end
    end
    X(i)=noFailures;
end

UX=unique(X);
nX=hist(X, length(UX)); % 'hist' counts nr. of occurences
rel_freq=nX/S;

plot(UX, rel_freq, '*');
hold on;
plot(0:max(UX), nbinpdf(0:max(UX), n, p), '^');
hold off;

title("Pascal Model");
legend("Simulation", "Theoretical")