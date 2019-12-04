directory = 'ISO1200\';
file_list = dir([directory 'nightshot_input*.jpg']);

im_sum = im2double(imread([directory file_list(1).name]));
count = length(file_list);

for i=2:count
    im = im2double(imread([directory file_list(i).name]));
    figure, imshow(im);
    im_sum = im_sum + im;
end

dst = im_sum./count;
imwrite(dst, 'sum.jpg');
