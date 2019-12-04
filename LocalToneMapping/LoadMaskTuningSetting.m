function mask_tuning = LoadMaskTuningSetting(json_file)

    txt = fileread(json_file);
    mask_tuning = jsondecode(txt);

    % mask_profile = 'FullSize';
%     mask_profile = 'PreviewSize';
%     
%     %mask_tuning.filter_select = 'GuidedFilter';
%     mask_tuning.filter_select = 'MedianFilter';
%     
%     switch lower(mask_profile)
%         case 'previewsize'
%             mask_tuning.median_filter_size = [4 4];
%         case 'fullsize'
%             mask_tuning.median_filter_size = [12 12];
%         otherwise
%             mask_tuning.median_filter_size = [4 4];
%     end
%     
%     mask_tuning.guided_filter_size = [4 4];
%     mask_tuning.guided_filter_smooth = 0.01;
end
