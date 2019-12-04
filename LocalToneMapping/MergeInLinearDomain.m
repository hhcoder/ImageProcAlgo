% Merge images in linear domain
% Input:
%   src_imgs
% Output:
%   rgb
function [merged, weight, gamma_merged] = MergeInLinearDomain(src_imgs, option)
    if nargin < 2
        option = '';
    end

    degamma_images = GammaInverse(src_imgs);
    
    % Default here for now
    select = 'flat';

    if size(degamma_images, 4) > 1
        switch lower(select)
            case 'flat'
                MergeWeightFxn = @FlatCurve;
            case 'sin'
                MergeWeightFxn = @SinCurve;
            case 'gaussian'
                MergeWeightFxn = @GaussianCurve;
        end

        weight = MergeWeightFxn(degamma_images);

        S = sum(weight, 4);
        S = S + double(S==0); %Remove all zeros

        merged = sum(degamma_images .* weight, 4) ./ S;
        
        if strcmp(option, 'display')
            figure,
            x = 0:0.01:1;
            plot(x, WeightMapping(x));
            axis([-0.1 1.1 -0.1 1.1]);
            title(['Merge Weighting Curve - ' upper(select)]);
        end

    else
        weight = ones(size(degamma_images));
        merged = degamma_images;
    end
    
    gamma_merged = GammaForward(merged);

end

function v = FlatCurve(x)
    v = double(x>=0.0&x<=1.0);
end
function v = GaussianCurve(x)
    mu=0.5;
    tho=0.2;
    v = exp(-1/2.*((x-mu)./tho).^2);
end

function v = SinCurve(x)
    v = sin(x*pi);
end
