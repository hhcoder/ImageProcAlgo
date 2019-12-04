% Apply Gain Map
% Input:
%   rgb
%   gain_map
%   setting.dst
% Output:
%   rgb_tm
function rgb_ret = ApplyGainMap(rgb, y_ratio)
   
    rgb_ret = zeros(size(rgb));
    rgb_ret(:,:,1) = y_ratio .* rgb(:,:,1);
    rgb_ret(:,:,2) = y_ratio .* rgb(:,:,2);
    rgb_ret(:,:,3) = y_ratio .* rgb(:,:,3);    

end

