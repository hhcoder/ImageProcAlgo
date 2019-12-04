function dst = BlackLevelSubtraction(src)

    black_level = 0.05;
    
    sub = src - black_level;
    
    sub_idx = sub < 0;
    sub(sub_idx) = 0;
    
    dst = sub.* 1.00 / (1.00 - black_level);
    
    figure,
    subplot(1,2,1);
    imshow(src);
    title('Before Black Level');

    subplot(1,2,2);
    imshow(dst);
    title('After Black Level');


end