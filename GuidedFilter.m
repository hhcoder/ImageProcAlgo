function q = GuidedFilter(I, P)
    q = GuidedFilterImpl(I, P);
    figure,
    subplot(2,2,1), imshow(I), title('Original Image');
    subplot(2,2,3), imshow(imfilter(I, fspecial('gaussian', 5, 1.5))), title('Gaussian Filtered');
    subplot(2,2,4), imshow(q), title('Filtered Image');
    
    figure,
    imshow(imabsdiff(I, q)), title('Difference between filtered image and original image');

end

function q = GuidedFilterImpl(I, P)
    epislon = 1.4.^2;

    mean_I = FxnMean(I);
    mean_P = FxnMean(P);
    corr_I = FxnMean(I.*I);
    corr_I_P = FxnMean(I.*P);
    var_I = corr_I - mean_I .* mean_I;
    cov_I_P = corr_I_P - mean_I .* mean_P;
    a = cov_I_P ./ (var_I + epislon);
    b = mean_P - a.*mean_I;
    mean_a = FxnMean(a);
    mean_b = FxnMean(b);
    q = mean_a .* I + mean_b;
end

function ret = FxnMean(I)
    ret = imfilter(I, fspecial('gaussian', 5, 0.5));
end