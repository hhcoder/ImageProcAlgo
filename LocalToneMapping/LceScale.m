function ret = LceScale(x, lce)
    switch lce.scale.select
        case 'bezier'
            ret.bezier_knee_x = lce.scale.bezier_knee_x;
            ret.bezier_knee_y = lce.scale.bezier_knee_y;
            [ret.x, ret.y] = BezierCurve( ...
                            ret.bezier_knee_x, ...
                            ret.bezier_knee_y, ...
                            x);
        otherwise
            assert('Not implemented yet!');
    end
end
