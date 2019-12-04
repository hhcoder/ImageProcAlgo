function img_ret = ApplyCurveToImage(curve_func, curve_tuning, img)
    xx = -1:0.005:1;
    c = curve_func(xx, curve_tuning);
    img_ret = zeros(size(img));

    img_ret(1:end) = interp1(c.x, c.y, img(1:end));
end