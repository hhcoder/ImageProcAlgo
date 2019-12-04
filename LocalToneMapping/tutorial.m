function tutorial
    in_img = imread('../../InputSource/SunSetBacklit/20190905_183139.jpg');
    src_img = im2double(imresize(in_img, [480 640]));
    
    h = size(src_img,1);
    w = size(src_img,2);
    
    
    line_sec = src_img(2.*h./4,:,2);
    
    figure, 
    imshow(src_img); 
    hold on;
    line([0 w], [2.*h/4 2.*h/4]);
    hold off;
    
    figure, plot(line_sec);

end