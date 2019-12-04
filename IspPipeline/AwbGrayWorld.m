function dst = AwbGrayWorld(src, display)

if nargin < 2
    display = 'none';
end

    rsum = sum(sum(src(:,:,1)));
    gsum = sum(sum(src(:,:,2)));
    bsum = sum(sum(src(:,:,3)));
    
    g_over_r = gsum/rsum;
    g_over_b = gsum/bsum;
    
    dst = zeros(size(src));
    
    dst(:,:,1) = src(:,:,1)*g_over_r;
    dst(:,:,2) = src(:,:,2);
    dst(:,:,3) = src(:,:,3)*g_over_b;
    
if strcmp(display, 'all figure')
    figure,
    
    subplot(1,2,1);
    imshow(src);
    title('Before AWB');

    subplot(1,2,2);
    imshow(dst);
    title('After AWB');
end

end