# minOS - A minimal Operating System from scratch

## About

このリポジトリはx64フルスクラッチOS「minOS」のリポジトリです。

## 開発状況

現在はシリアル割り込みを実装し、コンソールで数個のコマンドが打てる状態です。

またスレッドを実装し、マルチタスクを実現する予定です。

## EDK2ビルド環境の構築

初回は `edk2/` 内で

```
$ ./edksetup.sh
$ source edksetup.sh
```

をするとよい。（ようわからん）

## Build error

EDK2のビルドエラーが出た場合はBaseTools C Tools Binaryがない可能性があります。
エラーログを見てもし該当するなら以下のコマンドを叩いてください。

`edk2/` 内で、

```
make -C BaseTools/Source/C
```
