BEGIN {
    split("", func_at)
    split("", addresess)
    i = 0
}

(FILENAME == "./kernel/kernel.map") && ($0 ~ /((\s)*)0x([(0-9),(a-f)]{16})((\s){16})/) {
    func_at[$1] = $2
    addresses[i] = $1
    i++
}

(FILENAME == "./kernel/dump.log") && ($1 ~ /[0-9,a-f]*:/) {
    addr_dump = substr($1, 0, length($1)-1)
    if (length(addr_dump) == 3) {
        addr_dump = "0" addr_dump
    }
    if (length(addr_dump) == 2) {
        addr_dump = "00" addr_dump
    }
    if (length(addr_dump) == 1) {
        addr_dump = "000" addr_dump
    }
    for (j = 0; j < i; j++) {
        match(addresses[j], /11.*([1-9,a-f][0-9,a-f]*)/)
        addr_match = substr(addresses[j], RSTART+2, RLENGTH-2)
        if (addr_dump == addr_match) {
            print addresses[j] ": " func_at[addresses[j]]
            print $0
            next
        }
    }
    print $0
}
