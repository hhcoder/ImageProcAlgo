function ret = LceCurve(x, lce)
    switch lce.curve.select
        case 'pchip'
            kx = lce.curve.pchip_knee_x;
            ky = lce.curve.pchip_knee_y;
            ret.pchip_knee_x = kx;
            ret.pchip_knee_y = ky;
            ret.x = x;
            ret.y = pchip(kx, ky, x);
        otherwise
            assert('Not implemented yet!');
    end
end

