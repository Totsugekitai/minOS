# paging

## paging modes and bits

以下のコントロールビットで制御される。

- CR0.WP(bit 16) と CR0.PG(bit 31)
- CR4 の PSE(bit 4), PAE(bit 5), PGE(bit 7), PCIDE(bit 17), SMEP(bit 20), SMAP(bit 21), PKE(bit 22)
- IA32_EFER MSR の LME(bit 8), NXE(bit 11)
- EFLAGS の AC(bit 18)

MOV命令をCR0に用いてCR0.PGを設定する。その前に、CR3に最初のページング構造の物理アドレスをふくめなければならないし、それを初期化しなければならない(Table4-7, 4-12)。

### three paging modes

CR0.PG: ページングのON,OFF
CR0.PE: プロテクションのON,OFF
CR4.PAE, IA32_EFER.LME: ページングモードの切り替え
    - CR0.PG = 1, CR4.PAE = 1, IA32_EFER.LME = 1 で4-level paging

ページングモードの違い
- リニアアドレスのサイズ
- 物理アドレスのサイズ
- ページのサイズ
- アクセス権限のサポート
- PCIDsのサポート
- プロテクションキーのサポート
    - 4-level では、リニアアドレスをプロテクションキーに関連付けるための機能を有効にできる

IA32_EFER.LMA = 1: IA-32eモードになる。直接設定不可。IA32_EFERに対してwrmsrしてもbit 10は無視される。

64-bit mode: 64-bitリニアアドレス。プロセッサはアドレスのbit 63:47 が同一であることを確認する。4-levelではbit 63:48は使用されない。

### paging mode enabling

以下の順番でビットを立てると4-levelのページングになる。

1. CR4.PAE = 1
2. IA32_EFER.LME = 1
3. CR0.PG = 1

- IA32_EFER.LMEはページングが有効になっている間は変更できない。wrmsrしたら#GP(0)が発生する。
- ページングモードにかかわらず、CR0.PGを下げるとページングが切れる。

### paging-mode modifiers

ページングモードは以下のビットで制御される。

- CR0.WP(bit 16)
    - スーパーバイザモードからの書き込みからページを保護するかどうか
    - WP = 0: スーパーバイザモードからの書き込みはread onlyのリニアアドレスに対して許可される
    - WP = 1: ユーザーモードからの書き込みはread onlyのリニアアドレスに対して許可されない
    - Section 4.6を見よ。
- CR4 の PSE(bit 4), PAE(bit 5), PGE(bit 7), PCIDE(bit 17), SMEP(bit 20), SMAP(bit 21), PKE(bit 22)
    - PSE: 32bit pagingで4MBのページサイズ
    - PGE: global pageの有無。
        - Section 4.10.2.4
    - PCIDE: process-context identifiers(PCIDs)
        - 4-levelの時は1にしておく
        - 論理プロセッサに対してリニアアドレスの計算の情報をキャッシュする。
        - Section 4.10.1
    - SMEP: スーパーバイザモードからの命令フェッチからのページの保護。
        - これが立っているとユーザーモードでアクセスできるリニアアドレスから、OSは命令をフェッチできなくなる
        - Section 4.6
- IA32_EFER.NXE(bit 11)