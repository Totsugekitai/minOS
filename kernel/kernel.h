struct video_info_t {
    unsigned long long fb;
    unsigned long long fb_size;
    unsigned int x_axis;
    unsigned int y_axis;
    unsigned int ppsl;
};

struct bootinfo_t {
    struct video_info_t vinfo;
};

void start_kernel(struct bootinfo_t *binfo);
