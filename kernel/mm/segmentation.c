#include <stdint.h>
#include <types/mmtypes.h>

uint64_t make_segment_descriptor(uint32_t type, uint32_t dpl,
        uint32_t null_flag)
{
    uint64_t segment_descriptor = 0;
    /* baseとlimitは無視されるので0でよい */
    // typeはbit43:41に入れる
    segment_descriptor |= (((uint64_t)type) << 41);
    // dplはbit46:45に入れる
    segment_descriptor |= (((uint64_t)dpl) << 45);
    
    // null_flag == 1でnull descriptor
    if (null_flag == 0) {
        // 他のデフォルト設定
        // G,L,P,S = 1、他は0
        segment_descriptor |= 0x00a0900000000000;
    } else {
        // null descriptorなのですべて0
        segment_descriptor = 0x0000000000000000;
    }

    return segment_descriptor;
}

