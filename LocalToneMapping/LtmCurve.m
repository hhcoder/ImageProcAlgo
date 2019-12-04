function ret = LtmCurve(x, ltm)
    switch ltm.curve.select
        case 'exponential'
            ret.x = x;
            ret.y = power(x,ltm.curve.exp_ratio) - x;
        case 'sin'
            ret.x = x;
            ret.y = 0.5 .* sin(pi.*Y);
        case 'bezier'
            ret.bezier_knee_x = ltm.curve.bezier_knee_x;
            ret.bezier_knee_y = ltm.curve.bezier_knee_y;
            [ret.x, ret.y] = BezierCurve( ...
                            ret.bezier_knee_x, ...
                            ret.bezier_knee_y, ...
                            x);
    end
end