function x = GammaForward(X, gamma_val)
    if nargin < 2
        gamma_val = 2.2;
    end
    
    x = Clamp(power(abs(X), (1./gamma_val)), 0, 1);
end

