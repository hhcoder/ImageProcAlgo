function ret = LtmScale(x, ltm)

    
    switch ltm.scale.select
        case 'simple exponential'
            ret.x = x;
            ret.y = exp(-(x.^2)./0.3);
        case 'bezier'
            ret.bezier_knee_x = ltm.scale.bezier_knee_x;
            ret.bezier_knee_y = ltm.scale.bezier_knee_y;

            [ret.x, ret.y] = BezierCurve( ...
                            ret.bezier_knee_x, ...
                            ret.bezier_knee_y, ...
                            x);
    end
end
