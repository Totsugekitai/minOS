# minOS - A minimal Operating System from scratch

ぐちゃぐちゃになってきたので書き直しました。
->
[minOSv2](https://github.com/Totsugekitai/minOSv2)

多分もうこっちはいじらないと思います。

## About

このリポジトリはx64フルスクラッチOS「minOS」のリポジトリです。

## 開発状況

現在は

- シリアル割り込みを実装し、コンソールで数個のコマンドが打てる
- スレッドを実装し、マルチタスクを実現
- メモリの確保と解放の関数を実装
- AHCIを通してSATA接続されたストレージが読み書きできる

となっています。

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
