extern void reset_cr0_pg(int);
extern void set_cr4_pae(int);

void init_paging(void)
{
    int cr0_pg = 0x80000000;
    int cr4_pae = 0x10;
    int ia32_efer_lme = 0x80;

    reset_cr0_pg(cr0_pg);
    set_cr4_pae(cr4_pae);
    set_ia32_efer_lme(ia32_efer_lme);
    set_cr0_pg(cr0_pg);

    return;
}


