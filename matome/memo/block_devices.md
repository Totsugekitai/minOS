# ブロックデバイスについてのメモ

## ブロックデバイスへの接続方法

PCI に接続されているのでそこから読む。
PCI Configuration Spaceにアクセスして頑張る。

## PCI Configuration Space

I/O空間上のCONFIG_ADDRESS(0x0cf8)とCONFIG_DATA(0x0cfc)レジスタを用いてConfiguration Registerを読み書き。

詳細は[osdev-jpのwiki](https://github.com/osdev-jp/osdev-jp.github.io/wiki/PCI-Memo)に載っている。

[OSDev.orgのwiki](https://wiki.osdev.org/PCI)も読んで頑張る。