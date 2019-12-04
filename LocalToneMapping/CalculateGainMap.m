% CalculateGainMap
% Input:
%  Y
%  y_tm
% Output:
%  gain_map
function gain_map = CalculateGainMap(Y, y_tm)
    
    % Remove zero for later division
    y = Clamp(Y, 1/255, 1.0);

    % Calculate 2D gain map
    gain_map = y_tm./y;
end

