unsigned long pow(unsigned long num, unsigned long pow)
{
    unsigned long i, ans = 1;
    for (i = 0; i < pow; i++) {
        ans *= num;
    }
    return ans;
}
