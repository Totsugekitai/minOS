#include <stdint.h>
#include <util/util.h>
#include <device/device.h>

#define PORT    0x3f8

extern uint8_t keycode;

/* シリアル通信 */
void init_serial(void)
{
    io_cli();
    io_outb(PORT + 1, 0x00);
    io_outb(PORT + 3, 0x80);
    io_outb(PORT + 0, 0x01);
    io_outb(PORT + 1, 0x00);
    io_outb(PORT + 3, 0x03);
    io_outb(PORT + 2, 0xC7);
    io_outb(PORT + 4, 0x0B);
    io_outb(PORT + 1, 0x0d); // 割り込み許可
    io_sti();
}

uint32_t serial_received(void)
{
    return io_inb(PORT + 5) & 1;
}

void wait_serial_input(void)
{
    while (keycode == 0) {
        asm volatile("hlt");
    }
    puts_serial("wait_serial_input after while\n");
}

uint8_t receive_serial_input(void)
{
    while (keycode == 0) {
        asm volatile("hlt");
    }
    puts_serial("receive_serial_input after while\n");

    uint8_t c = keycode;
    puts_serial("receive_serial_input c = keycode\n");

    return c;
}

uint8_t read_serial(void)
{
    while (serial_received() == 0);
    return io_inb(PORT);
}

uint8_t serial_thr_empty(void)
{
    return io_inb(PORT + 5) & 0x20;
}

void write_serial(uint8_t c)
{
    while (serial_thr_empty() == 0);
    io_outb(PORT, c);
}

void puts_serial(const char *s)
{
    uint8_t i = 0;
    io_cli();
    while (s[i] != 0x00) {
        write_serial(s[i]);
        i++;
    }
    io_sti();
}

void putnum_serial(uint64_t num)
{
    uint64_t i;
    uint64_t numarr[16];
    char numchararr[19];
    for (i = 0; i < 16; i++) {
        numarr[i] = (num & (pow(2, 0 + i * 4) + pow(2, 1 + i * 4) + pow(2, 2 + i * 4) + pow(2, 3 + i * 4))) >> i * 4;
    }
    numchararr[0] = '0';
    numchararr[1] = 'x';
    numchararr[18] = '\0';
    for (i = 0; i < 16; i++)
    {
        if (numarr[i] == 0x0)
        {
            numchararr[17 - i] = '0';
        }
        if (numarr[i] == 0x1)
        {
            numchararr[17 - i] = '1';
        }
        if (numarr[i] == 0x2)
        {
            numchararr[17 - i] = '2';
        }
        if (numarr[i] == 0x3)
        {
            numchararr[17 - i] = '3';
        }
        if (numarr[i] == 0x4)
        {
            numchararr[17 - i] = '4';
        }
        if (numarr[i] == 0x5)
        {
            numchararr[17 - i] = '5';
        }
        if (numarr[i] == 0x6)
        {
            numchararr[17 - i] = '6';
        }
        if (numarr[i] == 0x7)
        {
            numchararr[17 - i] = '7';
        }
        if (numarr[i] == 0x8)
        {
            numchararr[17 - i] = '8';
        }
        if (numarr[i] == 0x9)
        {
            numchararr[17 - i] = '9';
        }
        if (numarr[i] == 0xa)
        {
            numchararr[17 - i] = 'a';
        }
        if (numarr[i] == 0xb)
        {
            numchararr[17 - i] = 'b';
        }
        if (numarr[i] == 0xc)
        {
            numchararr[17 - i] = 'c';
        }
        if (numarr[i] == 0xd)
        {
            numchararr[17 - i] = 'd';
        }
        if (numarr[i] == 0xe)
        {
            numchararr[17 - i] = 'e';
        }
        if (numarr[i] == 0xf)
        {
            numchararr[17 - i] = 'f';
        }
    }
    puts_serial(numchararr);
}

/* PS/2 */
uint8_t ps2_received(void)
{
    while (!(read_kbd_status() & 0x01));
    return read_kbd_signal();
}

uint8_t read_ps2(void)
{
    uint8_t keycode;
    while ((keycode = ps2_received()) & 0x80);
    return keycode;
}

// JISキーボード対応
uint8_t map_scan_to_ascii_set1(uint8_t scan, uint8_t *shift)
{
    if (scan == 0x2a || scan == 0x36) {
        *shift = 1;
        return 0x00;
    }
    if (scan == 0xaa || scan == 0xb6) {
        *shift = 0;
        return 0x00;
    }

    if (*shift == 0) {
        switch (scan)
        {
        case 0x1e:
            return 'a';
        case 0x30:
            return 'b';
        case 0x2e:
            return 'c';
        case 0x20:
            return 'd';
        case 0x12:
            return 'e';
        case 0x21:
            return 'f';
        case 0x22:
            return 'g';
        case 0x23:
            return 'h';
        case 0x17:
            return 'i';
        case 0x24:
            return 'j';
        case 0x25:
            return 'k';
        case 0x26:
            return 'l';
        case 0x32:
            return 'm';
        case 0x31:
            return 'n';
        case 0x18:
            return 'o';
        case 0x19:
            return 'p';
        case 0x10:
            return 'q';
        case 0x13:
            return 'r';
        case 0x1f:
            return 's';
        case 0x14:
            return 't';
        case 0x16:
            return 'u';
        case 0x2f:
            return 'v';
        case 0x11:
            return 'w';
        case 0x2d:
            return 'x';
        case 0x15:
            return 'y';
        case 0x2c:
            return 'z';
        case 0x0b:
            return '0';
        case 0x02:
            return '1';
        case 0x03:
            return '2';
        case 0x04:
            return '3';
        case 0x05:
            return '4';
        case 0x06:
            return '5';
        case 0x07:
            return '6';
        case 0x08:
            return '7';
        case 0x09:
            return '8';
        case 0x0a:
            return '9';
        case 0x0c:
            return '-';
        case 0x0d:
            return '^';
        case 0x7d:
            return '\\';
        case 0x2b:
            return ']';
        case 0x0e:
            return 0x08; // バックスペース
        case 0x39:
            return ' ';
        case 0x1c:
            return 0x0a; // LF(改行)
        case 0x1a:
            return '@';
        case 0x1b:
            return '[';
        case 0x27:
            return ';';
        case 0x28:
            return ':';
        case 0x33:
            return ',';
        case 0x34:
            return '.';
        case 0x35:
            return '/';
        case 0x73:
            return '\\';
        default:
            break;
        }
    } else {
        switch (scan)
        {
        case 0x1e:
            return 'A';
        case 0x30:
            return 'B';
        case 0x2e:
            return 'C';
        case 0x20:
            return 'D';
        case 0x12:
            return 'E';
        case 0x21:
            return 'F';
        case 0x22:
            return 'G';
        case 0x23:
            return 'H';
        case 0x17:
            return 'I';
        case 0x24:
            return 'J';
        case 0x25:
            return 'K';
        case 0x26:
            return 'L';
        case 0x32:
            return 'M';
        case 0x31:
            return 'N';
        case 0x18:
            return 'O';
        case 0x19:
            return 'P';
        case 0x10:
            return 'Q';
        case 0x13:
            return 'R';
        case 0x1f:
            return 'S';
        case 0x14:
            return 'T';
        case 0x16:
            return 'U';
        case 0x2f:
            return 'V';
        case 0x11:
            return 'W';
        case 0x2d:
            return 'X';
        case 0x15:
            return 'Y';
        case 0x2c:
            return 'Z';
        case 0x02:
            return '!';
        case 0x03:
            return '"';
        case 0x04:
            return '#';
        case 0x05:
            return '$';
        case 0x06:
            return '%';
        case 0x07:
            return '&';
        case 0x08:
            return '\'';
        case 0x09:
            return '(';
        case 0x0a:
            return ')';
        case 0x0c:
            return '=';
        case 0x0d:
            return '~';
        case 0x7d:
            return '|';
        case 0x2b:
            return '}';
        case 0x0e:
            return 0x08; // バックスペース
        case 0x39:
            return ' '; // スペース
        case 0x1c:
            return 0x0a; // LF(改行)
        case 0x1a:
            return '`';
        case 0x1b:
            return '{';
        case 0x27:
            return '+';
        case 0x28:
            return '*';
        case 0x33:
            return '<';
        case 0x34:
            return '>';
        case 0x35:
            return '?';
        case 0x73:
            return '_';
        default:
            break;
        }
    }
    return 0x00;
}

uint8_t map_scan_to_ascii_set2(uint8_t scan, uint8_t *shift)
{
    if (scan == 0x2a || scan == 0x36) {
        *shift = 1;
        return 0x00;
    }
    if (scan == 0xaa || scan == 0xb6) {
        *shift = 0;
        return 0x00;
    }

    if (*shift == 0) {
        switch (scan)
        {
        case 0x1e:
            return 'a';
        case 0x30:
            return 'b';
        case 0x2e:
            return 'c';
        case 0x20:
            return 'd';
        case 0x12:
            return 'e';
        case 0x21:
            return 'f';
        case 0x22:
            return 'g';
        case 0x23:
            return 'h';
        case 0x17:
            return 'i';
        case 0x24:
            return 'j';
        case 0x25:
            return 'k';
        case 0x26:
            return 'l';
        case 0x32:
            return 'm';
        case 0x31:
            return 'n';
        case 0x18:
            return 'o';
        case 0x19:
            return 'p';
        case 0x10:
            return 'q';
        case 0x13:
            return 'r';
        case 0x1f:
            return 's';
        case 0x14:
            return 't';
        case 0x16:
            return 'u';
        case 0x2f:
            return 'v';
        case 0x11:
            return 'w';
        case 0x2d:
            return 'x';
        case 0x15:
            return 'y';
        case 0x2c:
            return 'z';
        case 0x0b:
            return '0';
        case 0x02:
            return '1';
        case 0x03:
            return '2';
        case 0x04:
            return '3';
        case 0x05:
            return '4';
        case 0x06:
            return '5';
        case 0x07:
            return '6';
        case 0x08:
            return '7';
        case 0x09:
            return '8';
        case 0x0a:
            return '9';
        case 0x0c:
            return '-';
        case 0x0d:
            return '^';
        case 0x7d:
            return '\\';
        case 0x2b:
            return ']';
        case 0x0e:
            return 0x08; // バックスペース
        case 0x39:
            return ' ';
        case 0x1c:
            return 0x0a; // LF(改行)
        case 0x1a:
            return '@';
        case 0x1b:
            return '[';
        case 0x27:
            return ';';
        case 0x28:
            return ':';
        case 0x33:
            return ',';
        case 0x34:
            return '.';
        case 0x35:
            return '/';
        case 0x73:
            return '\\';
        default:
            break;
        }
    } else {
        switch (scan)
        {
        case 0x1e:
            return 'A';
        case 0x30:
            return 'B';
        case 0x2e:
            return 'C';
        case 0x20:
            return 'D';
        case 0x12:
            return 'E';
        case 0x21:
            return 'F';
        case 0x22:
            return 'G';
        case 0x23:
            return 'H';
        case 0x17:
            return 'I';
        case 0x24:
            return 'J';
        case 0x25:
            return 'K';
        case 0x26:
            return 'L';
        case 0x32:
            return 'M';
        case 0x31:
            return 'N';
        case 0x18:
            return 'O';
        case 0x19:
            return 'P';
        case 0x10:
            return 'Q';
        case 0x13:
            return 'R';
        case 0x1f:
            return 'S';
        case 0x14:
            return 'T';
        case 0x16:
            return 'U';
        case 0x2f:
            return 'V';
        case 0x11:
            return 'W';
        case 0x2d:
            return 'X';
        case 0x15:
            return 'Y';
        case 0x2c:
            return 'Z';
        case 0x02:
            return '!';
        case 0x03:
            return '"';
        case 0x04:
            return '#';
        case 0x05:
            return '$';
        case 0x06:
            return '%';
        case 0x07:
            return '&';
        case 0x08:
            return '\'';
        case 0x09:
            return '(';
        case 0x0a:
            return ')';
        case 0x0c:
            return '=';
        case 0x0d:
            return '~';
        case 0x7d:
            return '|';
        case 0x2b:
            return '}';
        case 0x0e:
            return 0x08; // バックスペース
        case 0x39:
            return ' '; // スペース
        case 0x1c:
            return 0x0a; // LF(改行)
        case 0x1a:
            return '`';
        case 0x1b:
            return '{';
        case 0x27:
            return '+';
        case 0x28:
            return '*';
        case 0x33:
            return '<';
        case 0x34:
            return '>';
        case 0x35:
            return '?';
        case 0x73:
            return '_';
        default:
            break;
        }
    }
    return 0x00;
}

