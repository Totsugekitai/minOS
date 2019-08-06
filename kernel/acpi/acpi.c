#include <stdint.h>
#include <util/util.h>
#include <acpi/acpi.h>

extern uint64_t num_sdts;
extern struct xsd_table *xsdt;

void *get_sdt(char *sig)
{
    for (uint64_t i = 0; i < num_sdts; i++) {
        if (strncmp(sig, xsdt->entry[i]->signature, 4)) {
            return xsdt->entry[i];
        }
    }
    return (void *)0;
}
