function ltm_result = Ltm(Y, m, tuning)
    ltm_result = LtmCurve(Y, tuning) .* LtmScale(m, tuning);
end