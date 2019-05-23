# minOS - A minimal Operating System from scratch

## About

このリポジトリはx64フルスクラッチOS「minOS」のリポジトリです。

## Build error

EDK2のビルドエラーが出た場合はBaseTools C Tools Binaryがない可能性があります。
エラーログを見てもし該当するなら以下のコマンドを叩いてください。

`edk2/` 内で、

```
make -C BaseTools/Source/C
```
