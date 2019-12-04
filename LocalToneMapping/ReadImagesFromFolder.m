% ReadImagesFromFolder
% Input:
%   src
%   proc
% Output:
%   imgs
function imgs = ReadImagesFromFolder(src, proc)    
    src_folder = src.folder_path;

    src_files = dir([src_folder '*.' src.img_ext]);
    
    assert( ~isempty(src_files), ['No ' src.img_ext ' files exists in folder:  ' src_folder] );
    
    first_img = imread([src_folder src_files(1).name]);

    if isfield(proc, 'resize_dim')
        height = proc.resize_dim(1);
        width = proc.resize_dim(2);
    else
        height = size(first_img, 1);
        width = size(first_img, 2);
    end
    channels = size(first_img,3);
    count = length(src_files);
    
    imgs = zeros([height, width, channels, count]);
    
    if count == 1
        resized = imresize(first_img, [height width]);
        imgs = double(resized)./255.0;
    else
        for i=1:count
            origin = imread([src_folder src_files(i).name]);
            resized = imresize(origin, [height width]);
            imgs(:,:,:,i) = double(resized)./255.0;
        end
    end
    
end