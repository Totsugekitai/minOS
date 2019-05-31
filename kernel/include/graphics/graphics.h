// ulはupper leftの略
void draw_square(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        unsigned int x_len, unsigned int y_len, struct video_info *vinfo);
void putchar(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char c);
void putstr(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char *s);

