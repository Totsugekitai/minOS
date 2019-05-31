struct pix_format {
    unsigned char bl;
    unsigned char gr;
    unsigned char re;
    unsigned char _reserved;
};

struct video_info {
    struct pix_format *fb;
    unsigned long fb_size;
    unsigned int x_axis;
    unsigned int y_axis;
    unsigned int ppsl;
};

struct bootinfo {
    struct video_info vinfo;
};

