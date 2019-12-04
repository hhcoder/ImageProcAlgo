import sys, getopt
import numpy as np
import scipy.misc
from PIL import Image

def main(argv):
    inputfile = ''
    outputfile = ''
    width = 0
    height = 0
    try:
        opts, args = getopt.getopt(argv,'i:w:h:o:',["ifile=","width=","height=", "ofile="])
    except getopt.GetoptError:
        print 'show_image.py -i <inputfile> -w <width> -h <height> -o <outputfile>'
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-v':
            print 'show_image.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-w", "--width"):
            width = int(arg)
        elif opt in ("-h", "--height"):
            height = int(arg)
        elif opt in ("-o", "--output"):
            outputfile = arg

    print 'Input file is ', inputfile
    print 'Width is ', width
    print 'Height is ', height
    print 'Output PNG is ', outputfile

    length = width * height

    raw_buffer = np.fromfile(inputfile, dtype=np.uint8, count=length)

    np_8bit = np.reshape(raw_buffer, (height, width))

    im_8bit = np_8bit.astype(np.uint8);

    img = Image.fromarray(im_8bit);

    img.save(outputfile);

if __name__ == "__main__":
    main(sys.argv[1:])

