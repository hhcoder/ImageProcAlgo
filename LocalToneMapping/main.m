%% Tone Mapping Algorithm Development

    close all
    clear
    clc

%% Load Default Settings

%    folder = 'ShortExposure';
%    folder = 'OnlineExample';
    folder = 'OfficeWindow';
%    folder = 'OutdoorPatio';
%    folder = 'SunSetBacklit';
    
    input_setting = LoadInputSetting(folder);
    proc_setting = LoadProcSetting();

%% Read Images from Specified Folder

    src_imgs = ReadImagesFromFolder(input_setting, proc_setting);
    
    PlotImageGrids(src_imgs, 'Original Images');

%% Merge images in linear domain

    [rgb_linear, weight, rgb_gamma] = MergeInLinearDomain(src_imgs);

     figure, imshow(rgb_gamma), title('Merged RGB');
     PlotImageGrids(rgb_linear, 'Inverse Gamma Images');
     PlotImageGrids(weight, 'Merge Weight Flat');

%% Load Tone Mapping Tuning Parameters

    % Load tuning parameters
    default_tuning_file_path = './tuning_tm.json';

    tuning = LoadTmTuningSetting(default_tuning_file_path);
    
    PlotAllToneMappingCurves(tuning.tm);

%% Calculate Y

    Y = CalculateY(rgb_linear, tuning.colortrans);

%% Calculate Mask

    m = CalculateMask(Y, tuning.mask);
      
     figure, subplot(2,1,1), imshow(Y), title('Y');
     subplot(2,1,2), imshow(m), title(['Mask: ' tuning.mask.filter_select]);

%% Implement Tone Mapping
    
    [y_tm, y_ltm, y_gtm] = CalculateToneMapping(Y, m, tuning.tm);
    
%     figure;
%     subplot(2,2,1), imshow(y_ltm), title('Y Ltm Result');
%     subplot(2,2,2), imshow(y_gtm), title('Y Gtm Result');
%     subplot(2,2,3), imshow(y_tm), title('Y LTM + GTM Result');


    % Calculate Gain Map based on Tone Mapping Result
    gain_map = CalculateGainMap(Y, y_tm);

    %figure, imshow(gain_map./max(max(gain_map))), title('Gain Map');

    % Apply Gain Map to rgb
    rgb_tm = ApplyGainMap(rgb_linear, gain_map);

    % Plot intermediate result
    figure,
    subplot(2,1,1), imshow(rgb_gamma), title('Merged RGB');
    subplot(2,1,2), imshow(GammaForward(rgb_tm)), title('Ltm + Gtm RGB');
    
    % Write itermediate results out
    dst = LoadOutputSetting();

    % Output intermediate result
    for i=1:size(src_imgs,4)
        fpath = sprintf('%s%02d%s', dst.orig_file_name, i, dst.ext);
        imwrite(src_imgs(:,:,:,i), fpath);
    end
    imwrite(rgb_linear, dst.linear_merged_rgb_path);
    imwrite(m, dst.mask_file_path);
    imwrite(rgb_gamma, dst.merged_rgb_path);
    imwrite(GammaForward(rgb_tm), dst.ltm_rgb_path);
