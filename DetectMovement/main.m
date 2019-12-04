close all
clear
clc

in_width = 1440;
in_height = 1080;

src_folder = '../../Source/Jogging/155200/';
yuv_ext = '*.yuv';

%src_folder = '../../Source/Jogging/155223/';
%yuv_ext = '*.YUV420NV21';

%src_folder = '../../Source/Jogging/155247/';
%yuv_ext = '*.yuv';

src_files = dir([src_folder yuv_ext]);

%src_file = 'p[0xd1680000]_req[1]_batch[0]_IPE[0]_port[10]_w[1440]_h[1080]_20190103_155200.yuv';

%yuv_count = length(src_files);

% TODO: fix the file naming problem
yuv_count = 99; 

prev_yuv = ReadYuv([src_folder src_files(1).name], in_width, in_height);
curr_yuv = ReadYuv([src_folder src_files(2).name], in_width, in_height);

diff_map = GenerateDiffMap(prev_yuv, curr_yuv);
prev_diff_map = diff_map;

figure,

for i=3:yuv_count
    a = 0.8;
    motion_map =  a.*diff_map + (1-a).*prev_diff_map;
    
    subplot(1,2,1);
    imshow(motion_map);
    subplot(1,2,2);
    imshow(ycbcr2rgb(curr_yuv));
    
    drawnow

    prev_diff_map = diff_map;

    prev_yuv = curr_yuv;
    curr_yuv = ReadYuv([src_folder src_files(i).name], in_width, in_height);

    diff_map = GenerateDiffMap(prev_yuv, curr_yuv);
end

