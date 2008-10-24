[SCE CONFIDENTIAL DOCUMENT]
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージには、Cell OS Lv-2 ツールチェーンのソースファイルが含ま
れています。このパッケージを使ってツールチェーンの再構築を行う場合には、
別パッケージで提供している、Cell OS Lv-2 ツールチェーンを、規定のディレ
クトリである/usr/local/cellにインストールしておくか、インストール
したディレクトリ名を環境変数CELLSDKに設定する必要があります。

スクリプトbuild.shを起動すると、Linux版のツールチェーンの再構築が行われ
ます。build.shは、build-ppu-lv2.shとbuild-spu-lv2.shを使用しています。ツ
ールチェーンをインストールするディレクトリと、再構築に使う作業用ディレ
クトリは、環境変数により変更することができます。詳細は各スクリプトのコ
メントを参照してください。
例:
    mkdir work/src
    unzip toolchain-src-2.5.0-GCC411.zip -d work/src
    mkdir work/build
    cd work/build
    ../src/build.sh
スクリプトbuild-mingw.shは、Windows版のツールチェーンを、Linux環境で再
構築するためものです。再構築には、MinGWランタイムが必要となります。以下
の二つのファイルをダウンロードして、mingwディレクトリに置いてください。
  mingw-runtime-3.3.tar.gz
    http://sourceforge.net/forum/forum.php?forum_id=372259
  w32api-2.5.tar.gz
    http://sourceforge.net/forum/forum.php?forum_id=352558
build-mingw.shは、build-lm.sh、build-mp-lv2.sh、build-ms-lv2.shを使用し
ています。ツールチェーンをインストールするディレクトリと、再構築に使う作
業用ディレクトリは、環境変数により変更することができます。詳細は各スクリ
プトのコメントを参照してください。

現在の仕様は暫定的なものであり、予告なく変更される場合があります。
----------------------------------------------------------------------
パッケージ構成
----------------------------------------------------------------------
Readme_j.txt
Readme_e.txt
build.sh                Linux版ツールチェーンの再構築用スクリプト
build-ppu-lv2.sh        Lv-2のPPUツールチェーンの再構築用スクリプト
build-spu-lv2.sh        Lv-2のSPUツールチェーンの再構築用スクリプト
build-mingw.sh          Windows版ツールチェーンの再構築用スクリプト
build-lm.sh             Linux->Windowsツールチェーンの再構築用スクリプト
build-mp-lv2.sh         Windows版のLv-2のPPUツールチェーンの再構築用スクリプト
build-ms-lv2.sh         Windows版のLv-2のSPUツールチェーンの再構築用スクリプト
diff-from-gcc           GCC 4.1.2およびBinutils 2.17からの差分情報
                        これには、当社による変更点のみが含まれます。
binutils/               GNU development toolsソースファイル
gcc/                    GNU Compiler Collection (GCC)ソースファイル
mingw/                  ダウンロードしたMinGWランタイムを置く場所

----------------------------------------------------------------------
使用許諾・制限
----------------------------------------------------------------------
このソフトウェアの使用許諾、使用制限は貴社と当社(株式会社ソニー・
コンピュータエンタテインメント)との間に締結されている契約に準じます。

----------------------------------------------------------------------
商標に関する注意書き
----------------------------------------------------------------------
パッケージ内に記載されている会社名、製品名は一般に各社の商標または
登録商標です。
なお、本文中に(R)、(TM)マークは明記していない場合があります。
