% Load tuning settings based on trail #
function tm_tuning = LoadTmTuningSetting(json_file)
%     tuning.tm.ltm_percentage = 0.95;
%     
%     % LTM Curve related tuning parameters
%     %tuning.ltm.curve.select = 'exponential';
%     tuning.ltm.curve.select = 'bezier';
%     tuning.ltm.curve.exp_ratio = 1/3;
%     tuning.ltm.curve.bezier_knee_x = [0.0  0.3  0.6  1.0];
%     tuning.ltm.curve.bezier_knee_y = [0.0  0.7  0.0  0.0];
% 
%     % LTM Scale related tuning parameters
%     tuning.ltm.scale.select = 'bezier';
%     tuning.ltm.scale.bezier_knee_x = [0.0  0.3  0.7  1.0];
%     tuning.ltm.scale.bezier_knee_y = [1.0  0.7  0.9  0.0];
% 
%     tuning.lce = 'reserved';
%     tuning.gtm = 'reserved';
    txt = fileread(json_file);
    tm_tuning = jsondecode(txt);
end