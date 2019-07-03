/* bss.c */
void init_bss(void);

/* gdt.c */
void init_gdt(void);
extern void load_gdt(struct segm_descriptor *);

