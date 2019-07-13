# Paging Cheat-Sheet

まとめのまとめです。

## Flow of Initialization

1. ページング構造を初期化
2. CR3にページング構造の物理アドレスを追加
3. 以下の手順でビットを立てる
    1. CR4.PAE = 1
    2. IA32_EFER.LME = 1
    3. CR4.PG = 1

## Initialization of Paging-Structures

### types of Paging-Structures

4-level Pagingの中でもページサイズが2MBのモデルを用いる。
以下の3つのページング構造がある。

- PML4
- PDP
- PD

### Flow of translation

リニアアドレスから物理アドレスの流れとしては、

              Linear  Address
        ↓       ↓      ↓           ↓
CR3 -> PML4 -> PDP -> PD -> Physical Address

である。

### Initialization of Paging-Tables

以下は各テーブルの初期化パラメータである。
なお物理アドレス空間のサイズは4GBとする。

- PML4 Table
    - size: 
