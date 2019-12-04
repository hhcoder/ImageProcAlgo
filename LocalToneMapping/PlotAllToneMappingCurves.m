function PlotAllToneMappingCurves(tm_tuning)

    ltm_curve_result = LtmCurve(0:0.01:1, tm_tuning.ltm);
    
    ltm_scale_result = LtmScale(0:0.01:1, tm_tuning.ltm);

    figure,
    subplot(2,1,1);
    PlotCurve(ltm_curve_result, 'LTM CURVE');
    subplot(2,1,2);    
    PlotCurve(ltm_scale_result, 'LTM SCALE');
    
    lce_curve_result = LceCurve(-1:0.01:1, tm_tuning.lce);
    
    lce_scale_result = LceScale(0:0.01:1, tm_tuning.lce);

    figure,
    subplot(2,1,1);
    PlotCurve(lce_curve_result, 'LCE CURVE');
    subplot(2,1,2);
    PlotCurve(lce_scale_result, 'LCE SCALE');
    
    %figure,
    %PlotCurve(x, Gtm(x, tuning.gtm), 'GtmCurve');

    PlotLtm3DCurve(tm_tuning);
    
    %PlotLce3DCurve(tm_tuning);        
end

function PlotCurve(curve_result, caption)
    x = curve_result.x;
    y = curve_result.y;

    hold on;
    
    plot(x, y, 'r');
    
    if max(x) ~= min(x) && max(y)~=min(y)
        ex = 0.1 .* max(x); % expand x range
        ey = 0.1 .* max(y); % expand y range
        axis([min(x)-ex max(x)+ex min(y)-ey max(y)+ey]);
    end

    if isfield(curve_result, 'bezier_knee_y') && isfield(curve_result, 'bezier_knee_y')
        knee_x = curve_result.bezier_knee_x;
        knee_y = curve_result.bezier_knee_y;

        plot(knee_x, knee_y, 'bo');

        line(knee_x(1:2), knee_y(1:2), 'LineStyle','--', 'Color', [0 1 0]);
        line(knee_x(end-1:end), knee_y(end-1:end), 'LineStyle','--', 'Color', [0 1 0]);
        legend('Bezier Curve', 'Knee Points', 'Derivative', 'Location', 'northwest');
    end
    
    if isfield(curve_result, 'pchip_knee_x')
        knee_x = curve_result.pchip_knee_x;
        knee_y = curve_result.pchip_knee_y;
        
        plot(knee_x, knee_y, 'bo');
        
        legend('PCHIP Curve', 'Knee Points', 'Location', 'northwest');
    end

    title(caption);
    hold off;   
end

function PlotLtm3DCurve(tuning)
    x1 = 0:0.005:1;
    y1 = LtmCurve(x1, tuning.ltm);
    
    x2 = 0:0.005:1;
    y2 = LtmScale(x2, tuning.ltm);
    
    z = zeros(length(x1), length(x2));
    
    for i=1:length(x2)
        z(:,i) = y1.y(:) .* y2.y(i);
    end
    
    figure, surf(z);

    xlabel('Y value');
    ylabel('Mask');
    zlabel('Plus Pixel Value');
end

function PlotLce3DCurve(tuning)
    [x1, x2] = meshgrid(0:0.005:1, 0:0.05:1);
    z = sign(x1-x2).*LceCurve(x1, tuning.lce).*LceScale(x2, tuning.lce);
    figure, surf(z);
    xlabel('Y value');
    ylabel('Mask');
    zlabel('Local Contrast Enhancement');
end

