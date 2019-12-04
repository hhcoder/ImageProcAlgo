function x = GammaInverse(X, gamma_val)
    if nargin < 2
        gamma_val= 2.2; %default value
    end
    x = power(X, gamma_val);
end
