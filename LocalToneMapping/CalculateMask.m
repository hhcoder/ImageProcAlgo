% CalculateMask
function m = CalculateMask(Y, mask_tuning)
    % Calculate mask based on Y
    switch lower(mask_tuning.filter_select)
        case 'guided_filter'
            m = imguidedfilter(Y, Y, ...
                        'NeighborhoodSize', tuning.guided_filter_size, ...
                        'DegreeOfSmoothing', tuning.guided_filter_smooth);
        case 'median_filter'
            m = medfilt2(Y, mask_tuning.median_filter_size);
        otherwise
            m = medfilt2(Y, mask_tuning.median_filter_size);
    end

end

