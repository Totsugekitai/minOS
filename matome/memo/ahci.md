# AHCI の使い方

## 初期化

初期化処理メモ（上から順番）
・ソフトウェア側でPxCMD.STを0にセットしハードウェアがPxCMD.CRを0にするのを待つ
・最低500msくらいは待つ
・（PxCMD.FREが1だった場合、これを0にしてハードウェアがPxCMD.FRを0にするのを待つ（最低500ms））
・CAP.NCSを読んで対応スロットを探す
・各対応スロットにおいて、PxCLBとPxFBのぶんのメモリを確保して0で埋める
・PxFBのメモリ領域を確保した後、PxCMD.FREを1にする
・各対応スロットに対して、PxSERRをクリアする。ビットに'1s'を書き込んでクリアする。
・PxISをクリアした後、IS.IPSをクリアする（順番が重要）
・PxIEレジスタをenableにする。またGHC.IEも1にセットする。

（必要なら追加の初期化をする、その際の注意）
・デバイスが動いている間は、PxCMD.STは1にセットしようとしない
　　　　・具体的には、PxTFD.STS.BSY=0,PxTFD.STS.DRQ=0,PxSSTS.DET=3hのときである
・PxTFDレジスタをenableにするために、PxSERR.DIAG.Xは0にクリアされてなければならない
