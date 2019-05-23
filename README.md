# minOS - A minimal Operating System from scratch

## About

このリポジトリはx64フルスクラッチOS「minOS」のリポジトリです。

## 開発状況

現在はOSローダの開発が一段落ついたところです。今後UEFIから必要な情報をもっと多く取りたいときは修正します。

## Build error

EDK2のビルドエラーが出た場合はBaseTools C Tools Binaryがない可能性があります。
エラーログを見てもし該当するなら以下のコマンドを叩いてください。

`edk2/` 内で、

```
make -C BaseTools/Source/C
```
