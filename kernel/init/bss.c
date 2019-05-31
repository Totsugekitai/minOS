extern int *__bss_start, *__bss_end;

void bss(void)
{
    int k, bss_size = (int)(__bss_end - __bss_start);
    for (k = 0; k < bss_size; k++) {
        __bss_start[k] = 0;
    }

    return;
}

