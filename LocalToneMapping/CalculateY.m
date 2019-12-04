%% Calculate Y
% Input: 
%   rgb
% Output
%   Y
function Y = CalculateY(linear_merged_rgb, ct_tuning)
    linear_merged_yuv = rgb2ycbcr(linear_merged_rgb);
    Y = linear_merged_yuv(:,:,1);
end
