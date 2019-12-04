function src = LoadInputSetting(src_folder)

    src_folder = [src_folder '/']; %attach / at end of trail

    
    % Input source related settings
    src.folder_path = ['../../InputSource/' src_folder];
    src.img_ext = 'jpg';
end