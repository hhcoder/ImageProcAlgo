function dst = Gamma(src)
    gamma_value = 2.2;
    dst = src.^(1/gamma_value);
    
    figure,
    subplot(1,2,1);
    imshow(src);
    title('Before Gamma');

    subplot(1,2,2);
    imshow(dst);
    title('After Gamma');
end