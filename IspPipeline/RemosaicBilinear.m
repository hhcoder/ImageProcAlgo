function bayer_image = RemosaicBilinear(raw_image, display)

if nargin < 2
    display = 'none';
end

r = zeros(size(raw_image));
gr = r;
gb = r;
b = r;

r(1:2:end, 1:2:end) = raw_image(1:2:end, 1:2:end);
gr(2:2:end, 1:2:end) = raw_image(2:2:end, 1:2:end);
gb(1:2:end, 2:2:end) = raw_image(1:2:end, 2:2:end);
b(2:2:end, 2:2:end) = raw_image(2:2:end, 2:2:end);
g = gr + gb;

kg = [0 1 0; 1 2 1; 0 1 0];
krb = [1 2 1; 2 4 2; 1 2 1];

r = imfilter(r, krb);
g = imfilter(g, kg);
b = imfilter(b, krb);

bayer_image = cat(3, r, g, b);

if strcmp(display, 'all figure')
    figure,
    imshow(r);
    title('r');
    figure,
    imshow(g);
    title('g');
    figure,
    imshow(b);
    title('b');
    figure,
    title('r+g+b');
    imshow(bayer_image);
end

end
