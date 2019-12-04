function [x, y] = BezierCurve(knee_x, knee_y, xx, option)

    if length(knee_x) ~= length(knee_y)
        assert('# of x and y knee points has to be the same');
    end
          
    pts = [knee_x knee_y];

    option = 'matrix';
    BCurve = SelectBCurve(length(knee_x), option);
    
    yy = BCurve(pts, xx);
        
    x = yy(1,:);
    y = yy(2,:);
end

%   Bezier curve implementation can be written in 
% polynomial expension form (FourthOrderBCurveExpensionForm)
% and matrix form (FourthOrderBCurveMatrixForm) for different orders;
% we implemented both.
function BCurve = SelectBCurve(len, option)
    switch(option)
        case 'matrix'
            switch(len)
                case 3
                    BCurve = @QuadraticBCurveMatrixForm;
                case 4
                    BCurve = @FourthOrderBCurveMatrixForm;
                case 5
                    BCurve = @FifthOrderBCurveMatrixForm;
                otherwise
                    assert('Supports only 3,4,5 knee points in Bezier curve for now');
            end
        case 'expansion'
            switch(len)
                case 3
                    BCurve = @QuadraticBCurveExpensionForm;
                case 4
                    BCurve = @FourthOrderBCurveExpensionForm;
                case 5
                    BCurve = @FifthOrderBCurveExpensionForm;
                case 6
                    BCurve = @SixthOrderBCurveExpensionForm;
                otherwise
                    assert('Supports only 3,4,5,6 knee points in Bezier curve for now');
            end
    end
end

function y = QuadraticBCurveExpensionForm(pts, x)
    t0 = 1 .* (1-x).^2 .* x.^0;
    t1 = 2 .* (1-x).^1 .* x.^1;
    t2 = 1 .* (1-x).^0 .* x.^2;
    
    y = kron(t0, pts(:,1)) + ...
        kron(t1, pts(:,2)) + ...
        kron(t2, pts(:,3));
end

function y = QuadraticBCurveMatrixForm(pts, x)
    % Bernstein Polynomial Matrix
    bm = [  1   -2   1; ...
           -2    2   0; ...
            1    0   0];
                                    
    y = [x.^2' x.^1' x.^0'] * bm * pts;
    y = y'; 
    %TODO: maybe we should force all y to be [N x 2] ?
end

function y = FourthOrderBCurveExpensionForm(pts, x)
    t0 = 1 .* (1-x).^3 .* x.^0;
    t1 = 3 .* (1-x).^2 .* x.^1;
    t2 = 3 .* (1-x).^1 .* x.^2;
    t3 = 1 .* (1-x).^0 .* x.^3;
    
    y = kron(t0, pts(:,1)) + ...
        kron(t1, pts(:,2)) + ...
        kron(t2, pts(:,3)) + ...
        kron(t3, pts(:,4));
end

function y = FourthOrderBCurveMatrixForm(pts, x)
    % Bernstein Polynomial Matrix
    bm = [ -1   3  -3   1; ...
            3  -6   3   0; ...
           -3   3   0   0; ...
            1   0   0   0];
                                    
    y = [x.^3' x.^2' x.^1' x.^0'] * bm * pts;
    y = y'; 
    %TODO: maybe we should force all y to be [N x 2] ?
end

function y = FifthOrderBCurveExpensionForm(pts, x)
    t0 = 1 .* (1-x).^4 .* x.^0;
    t1 = 4 .* (1-x).^3 .* x.^1;
    t2 = 6 .* (1-x).^2 .* x.^2;
    t3 = 4 .* (1-x).^1 .* x.^3;
    t4 = 1 .* (1-x).^0 .* x.^4;
    
    y = kron(t0, pts(:,1)) + ...
        kron(t1, pts(:,2)) + ...
        kron(t2, pts(:,3)) + ...
        kron(t3, pts(:,4)) + ...
        kron(t4, pts(:,5));
end

function y = FifthOrderBCurveMatrixForm(pts, x)
    % Bernstein Polynomial Matrix
    bm = [ 1  -4    6  -4   1; ...
          -4  12  -12   4   0;...
           6 -12    6   0   0; ...
          -4   4    0   0   0; ...
           1   0    0   0   0];
                                    
    y = [x.^4' x.^3' x.^2' x.^1' x.^0'] * bm * pts;
    y = y'; 
    %TODO: maybe we should force all y to be [N x 2] ?
end

function y = SixthOrderBCurveExpensionForm(pts, x)
    t0 = 1  .* (1-x).^5 .* x.^0;
    t1 = 5  .* (1-x).^4 .* x.^1;
    t2 = 10 .* (1-x).^3 .* x.^2;
    t3 = 10 .* (1-x).^2 .* x.^3;
    t4 = 5  .* (1-x).^1 .* x.^4;
    t5 = 1  .* (1-x).^0 .* x.^5;
    
    y = kron(t0, pts(:,1)) + ...
        kron(t1, pts(:,2)) + ...
        kron(t2, pts(:,3)) + ...
        kron(t3, pts(:,4)) + ...
        kron(t4, pts(:,5)) + ...
        kron(t5, pts(:,6));
end

