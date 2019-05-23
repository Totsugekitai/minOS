struct video_info_t {
    unsigned long long fb;
    unsigned long long fb_size;
};

struct bootinfo_t {
    struct video_info_t vinfo;
};

void start_kernel(struct bootinfo_t *binfo);
