% Plot a rows x cols subplot, where cols and rows
function SubplotNCols(jpg_images, caption, titles, cols)

    if nargin<4
        cols = 3;
    end

    figure('NumberTitle', 'off', 'Name', caption);

    jpg_count = size(jpg_images,4);
    for i=1:jpg_count
        if jpg_count~=1 % Subplot only when more than one images
            subplot(cols, ceil(jpg_count/cols), i);
        end
        
        imshow(jpg_images(:,:,:,i));

        if nargin > 2
            title(titles{i});
        end
    end

end