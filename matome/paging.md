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
    - SMAP: スーパーバイザモードからのデータアクセスからのページの保護。
        - 立てるとユーザーモードでアクセスできるデータにOSはアクセスできなくなる。
        - EFLAGS.ACを設定することによって上書きできる。
        - Section 4.6
    - PKE: リニアアドレスとプロテクションキーを関連付ける。
        - PKPUレジスタでそれぞれのプロテクションキーに対してリニアアドレスが読み書きできるか指定する。
        - Section 4.6
- IA32_EFER.NXE(bit 11)
    - NXE: 4-levelにおいてアクセス権限の実行と無効化を決める。
        - 立てると命令フェッチが指定されたリニアアドレスからされたふりをする（データの読み込みが許可されている場合）
        - Section 4.6

### emuration of paging features by cpuid

CPUID命令を使ったページングの疑似命令がつかえる。
4-levelページングに関係あるもののみまとめる。

- PAE: physical-address extension
    - CPUID.01H:EDX.PAE[bit 6] = 1の時、CR4.PAEを1にセットするとPAEページングモードになる。
- PGE: global-page support
    - CPUID.01H:EDX.PGE[bit 13] = 1の時、CR4.PGEを1にセットすると、global-pageが有効になる。
    - Section 4.10.2.4
- PAT: page-attribute table
    - CPUID.01H:EDX.PAT[bit 16] = 1の時、8-entry page-attribute table(PAT)が有効になる。
    - PATが有効の時、特定のページ構造エントリの3つのビットがメモリタイプをPATから選ぶ。
    - Section 4.9.2
- PSE-36: page-size extensions with 40-bit physical-address extension
    - CPUID.01H:ECX.PSE-36[bit 17] = 1のとき、PSE-36機構が有効になる。
    - 4MBのページを用いた変換を32-bitページングで物理アドレスが40-bitまで対応する。
- PCID: process-context identifiers
    - CPUID.01H:ECX.PCID[bit 17] = 1のときCR4.PCIDE = 1にすると、process-context identifiersが有効になる。
    - Section 4.10.1

## hierarchical paging structures: an overview

全てのページング構造は階層的。4-levelページングに関係あるもののみまとめる。
4-levelの詳細はSection 4.5にある。

全てのページング構造は4096bytesで、多数の個別のエントリを含む。
4-levelページングでは、エントリは64bits(8bytes)で、512エントリをそれぞれのデータ構造が保持している。

プロセッサはリニアアドレスの上位部分を用いてページング構造のエントリを特定する。
エントリの最後はリニアアドレスの変換先となる領域の物理アドレスを識別する。(page frameと呼ぶ)
リニアアドレスの下位部分は(page offsetと呼ぶ)、リニアアドレスの変換先となる、特定のアドレスを識別する。

どのページング構造でも、物理アドレスを含み、それは他のページング構造のアドレスとなっているか、もしくはページフレームのアドレスとなっている。
最初の例として、エントリは他のページング構造の"参照"と呼ばれ、その他は"ページマップ"と呼ばれる。

色々な変換に使われる最初のページング構造は、CR3レジスタ内部に物理アドレスで存在する。
リニアアドレスは次の段階的なプロシージャを用いて変換される。
リニアアドレスのある部分(初期は最も上位を占める)は、ページングエントリを選択する(CR3レジスタを用いて配置される)。

以下は例である(4KBページフレーム)。4-levelの例のみ記す。

それぞれのページング構造は512 = 2^9エントリから構成され、48bitリニアアドレスから変換には9bitが用いられる。
bits47:39は最初のページング構造を識別するのに使われ、bits38:30は2つ目のページング構造、bits29:21は3つ目、bits20:12は4つ目である。また、残りはページフレームを識別するのに使われる(Figure 4-8)。

変換プロセスはページフレームを識別して完了する。
ページフレームはオリジナルのリニアアドレスの変換の一部である。
しかし、一部のケースで、ページング構造は変更されることもあるので、変換プロセスはページフレームを識別してから行わなくてはならない。
これはプロセスが出くわした、ページング構造のエントリが、"not present"(P flag)かどうかを確認しなくてはならない。
P flagが0のときはリニアアドレスの変換は行われず、アドレスへのアクセスはpage-fault例外を引き起こす(Section 4.7)。

上の例で、あるページング構造のエントリがリニアアドレスに12bitしか残っていない場合に、pageを4KBのpageにマップする。以前に識別されたエントリは常に他のページング構造を参照する。


