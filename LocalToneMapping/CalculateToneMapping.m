% CalculateToneMapping
% Input:
%   Y
%   m
%   tuning
% Output:
%   y_ltm
%   y_gtm
%   y_tm

function [y_tm, y_ltm, y_gtm] = CalculateToneMapping(Y, m, tm_tuning)

    y_ltm_curve = ApplyCurveToImage(@LtmCurve, tm_tuning.ltm, Y);
    y_ltm_scale = ApplyCurveToImage(@LtmScale, tm_tuning.ltm, m);

    y_lce_curve = ApplyCurveToImage(@LceCurve, tm_tuning.lce, Y-m);
    y_lce_scale = ApplyCurveToImage(@LceScale, tm_tuning.lce, m);

    if tm_tuning.ltm.ltm_plus_lce
        y_ltm = Y + ...
                y_ltm_curve .* y_ltm_scale + ...
                y_lce_curve .* y_lce_scale;
    else
        y_ltm = Y + ...
                y_ltm_curve .* y_ltm_scale;
    end

    % GTM
    y_gtm = Gtm(Y, tm_tuning.gtm);

    % Mix GTM and LTM based on given percentage
    w = tm_tuning.ltm_percentage;

    y_tm = (y_ltm .* w) + (y_gtm .* (1-w));
end    

