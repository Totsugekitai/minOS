struct video_info_t {
        unsigned long long fb;
        unsigned long long fb_size;
};

struct bootinfo_t {
        struct video_info_t vinfo;
};

void start_kernel(void *t __attribute__((unused)), struct bootinfo_t *binfo)
{
        struct video_info_t vinfo = binfo->vinfo;
        unsigned long long *fbptr = (unsigned long long *)vinfo.fb;
        unsigned long long fbsize = vinfo.fb_size;

        unsigned long long i;
        for (i = 0; i < fbsize; i++) {
                fbptr[i] = 0xffffffff;
        }

        for (;;) asm volatile ("hlt");
}
