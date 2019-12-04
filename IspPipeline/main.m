close all
clear

    %raw_file_path = '../../SourceData/NightScene/IMG_20190102_084140.raw';
    raw_file_path = '../../SourceData/NightScene/IMG_20190222_192501.raw';

    raw_width = 4000;
    raw_height = 3000;
    raw_image = UnpackedRawImageRead(raw_file_path, raw_width, raw_height);

    dst = RemosaicBilinear(raw_image, 'all figure');

    dst = AwbGrayWorld(dst, 'all figure');
    
    dst = BlackLevelSubtraction(dst);
    
    dst = Gamma(dst);

    %mask = imfilter(raw_image, fspecial('gaussian', [5 5]));

    %LtmCurve(Y)

    % Y + LtmCurve(Y)*LtmScale(m) + Sing(Y-m) * LceCurve(|Y-m|) * LceScale(m);
