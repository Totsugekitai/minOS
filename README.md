# minOS - A minimal Operating System from scratch

## About

このリポジトリはx64フルスクラッチOS「minOS」のリポジトリです。

## 開発状況

現在は

- シリアル割り込みを実装し、コンソールで数個のコマンドが打てる
- スレッドを実装し、マルチタスクを実現
- メモリの確保と解放の関数を実装

これからの予定は

- 2次記憶媒体を使用できるようにし、ファイルシステムを実現


## EDK2ビルド環境の構築

初回は `edk2/` 内で

```
$ ./edksetup.sh
$ source edksetup.sh
```

をするとよい。

## Build error

EDK2のビルドエラーが出た場合はBaseTools C Tools Binaryがない可能性があります。
エラーログを見てもし該当するなら以下のコマンドを叩いてください。

`edk2/` 内で、

```
make -C BaseTools/Source/C
```
