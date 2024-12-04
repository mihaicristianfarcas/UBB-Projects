clear
clc
pause(1);

p=0;
while p < 0.05 || p > 0.95
    p=input("prob. of success (0.05 <= p <= 0.95)=");
end

for n=1:10
    k=0:n;
    px=binopdf(k, n, p);
    plot(k, px, '*');
    hold on;

    mu=n*p;
    sigma=sqrt(n*p*(1-p));
    kreal=0:0.01:n;
    normpx=normpdf(kreal, mu, sigma);
    plot(kreal, normpx);
    hold off;

    legend("binopdf", "normpdf");
    title("Approx. of Bino w/ Normal (n="+ n +")");

    pause(0.7);
end

p=1;
while p > 0.05
    p=input("prob. of success (0.05 >= p)=");
end

for n=30:50
    k=0:n;
    px=binopdf(k, n, p);
    plot(k, px, '*');
    hold on;

    lambda=n*p;
    poisspx = poisspdf(k, lambda);
    plot(k, poisspx, "o");
    hold off;

    legend("binopdf", "poisspdf");
    title("Approx. of Bino w/ Poisson (n="+ n +")");

    pause(0.7);
end