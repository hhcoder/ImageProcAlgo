function lce_result = Lce(Y, m, tuning)
    lce_result = sign(Y-m) .* LceCurve(abs(Y-m), tuning) .* LceScale(m, tuning);
end