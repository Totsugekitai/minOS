/* types */
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

/* graphics */
// ulはupper leftの略
void draw_square(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        unsigned int x_len, unsigned int y_len, struct video_info *vinfo);
void putchar(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char c);
void putstr(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char *s);

