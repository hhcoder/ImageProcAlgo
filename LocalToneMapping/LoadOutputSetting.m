function dst = LoadOutputSetting(trail)

    % Output result related settings
    if nargin == 0
         dst.folder_path = '../../OutputResult/temp/';
    else
        dst.folder_path = ['../../OutputResult/' trail];
    end
    
    dst_folder_path = dst.folder_path;

    if ~exist(dst_folder_path, 'dir') 
        mkdir(dst_folder_path); 
    else
        delete([dst_folder_path '*.*']);
    end
    
    dst.ext = '.tiff';
    
    ext = dst.ext;
    dst.mask_file_path = [dst_folder_path 'mask' ext];
    dst.merged_rgb_path = [dst_folder_path 'gamma_rgb' ext];
    dst.ltm_rgb_path = [dst_folder_path  'ltm_rgb' ext];
    dst.orig_file_name = [dst_folder_path  'src'];
    dst.linear_merged_rgb_path = [dst_folder_path  'linear_rgb' ext];
end