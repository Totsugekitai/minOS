#include <types/boottypes.h>
#include <graphics/graphics.h>

// グローバル変数の読み込み
extern struct pix_format blue;
extern struct pix_format white;
extern struct video_info *vinfo_global;

void gp_handler(void) {
    putstr(600, 70, white, blue, vinfo_global, "#GP");
    __asm__ volatile(
        "loop:     hlt           \n"
        "          jmp loop        "
    );
}
