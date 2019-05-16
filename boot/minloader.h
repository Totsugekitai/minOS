#include <stddef.h>

typedef struct {
        char kernel_start[17];
} conf;

typedef struct {
        void *fb;
        size_t fb_size;
        int x_axis, y_axis;
} video_info_t;

typedef struct {
        video_info_t *vinfo;
} bootinfo_t;

extern void start_kernel(bootinfo_t *bootinfo) __attribute__((__noreturn__));
