function diff_map = GenerateDiffMap(old_frame, new_frame)

    im2blur = @(I)( imfilter(I,fspecial('Gaussian', [3 3], 0.7)) );
    
    resize_ratio = 0.25;
    
    old_small_frame = imresize(old_frame(:,:,1), resize_ratio);
    new_small_frame = imresize(new_frame(:,:,1), resize_ratio);
    
    %absd = abs( im2blur(old_frame(:,:,1)) - im2blur(new_frame(:,:,1)) );
    absd = abs( im2blur(old_small_frame )- im2blur(new_small_frame) );
    
    alpha = 1.0;
        
    smooth_kernel = fspecial('gaussian', [5 5], 0.8);
    sharp_kernel = fspecial('gaussian', [5 5], 0.5);
    dog_kernel = sharp_kernel - smooth_kernel ;
    
    %diff_map = alpha .* imfilter(absd, fspecial('laplacian'));
    diff_map = alpha .* imfilter(absd, fspecial('gaussian'));
    %diff_map = alpha * imfilter(absd, dog_kernel);
    %diff_map = alpha .* entropyfilt(absd, ones([5 5]));
end