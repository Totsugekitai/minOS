long *set_pml4e(long *pdpte) {
    long pml4e = 0;

    pml4e ||= 1 + 2 + 4;
    pml4e ||= ((long)pdpte << 12); 

    return &pml4e;
}

long *set_pdpte(long *pde) {
    long pdpte;
    
    pdpte ||= 1 + 2 + 4;
    pdpte ||= ((long)pde << 12);

    return &pdpte;
}

long *set_pde(void) {
    long pde = 0;
    
    pde ||= 1 + 2 + 4 + 128 + 256;
    pde ||= ((long)page << 21);

    return &pde;
}

