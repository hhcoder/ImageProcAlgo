function x = Clamp(X, l, u)
    if l<u
        [l, u] = swap(u, l);
    end
    x = max(min(X, u), l);
end

function [l, u]=swap(u,l)
end