[SCE CONFIDENTIAL DOCUMENT]
Toolchain src GCC41
                    Copyright(C) 2011 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、PlayStation(R)3 Programmer Tool Toolchain パッケージ
で提供している GCC 4.1 のソースコードが含まれております。

GCC 4.1 は、GNU General Public License Version 3 (GPLv3) および 
GPLv3 第7条に従って追加された条項適用のソフトウェアを使用しており、
本パッケージで対象ソースコードを提供しております。
なお、ライセンスについては、下記 COPYING3 のファイルを参照してください。

  binutils/COPYING3
  gcc/COPYING3

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
diff-from-gcc           GCC 4.1.1およびBinutils 2.17からの差分情報
                        これには、当社による変更点のみが含まれます。
binutils/               GNU development toolsソースファイル
gcc/                    GNU Compiler Collection (GCC)ソースファイル
mingw/                  ダウンロードしたMinGWランタイムを置く場所

----------------------------------------------------------------------
ツールチェーンの再構築方法
----------------------------------------------------------------------
ツールチェーンの再構築を行う場合には、別パッケージで提供している、
PlayStation(R)3 Programmer Tool Toolchain を、
規定のディレクトリである /usr/local/cell にインストールしておくか、
インストールしたディレクトリ名を環境変数 CELLSDK に設定する必要があります。

スクリプト build.sh を起動すると、Linux 版のツールチェーンの再構築が行われ
ます。build.sh は、build-ppu-lv2.sh と build-spu-lv2.sh を使用しています。
ツールチェーンをインストールするディレクトリと、再構築に使う作業用ディレ
クトリは、環境変数により変更することができます。詳細は各スクリプトのコ
メントを参照してください。

例:
    mkdir work/src
    unzip toolchain-src-3.7.0-GCC411.zip -d work/src
    mkdir work/build
    cd work/build
    ../src/build.sh

スクリプト build-mingw.sh は、Windows 版のツールチェーンを、
Linux 環境で再構築するためものです。
再構築には、MinGW ランタイムが必要となります。以下の二つのファイルを
ダウンロードして、mingw ディレクトリに置いてください。
  mingw-runtime-3.3.tar.gz
  w32api-2.5.tar.gz
    http://ftp.jaist.ac.jp/pub/sourceforge/m/project/mi/mingw/OldFiles/

build-mingw.sh は、build-lm.sh、build-mp-lv2.sh、build-ms-lv2.sh を使用し
ています。ツールチェーンをインストールするディレクトリと、再構築に使う作
業用ディレクトリは、環境変数により変更することができます。
詳細は各スクリプトのコメントを参照してください。

----------------------------------------------------------------------
変更点
----------------------------------------------------------------------
パッケージに含まれるソースファイルは、以下のURLより配布されている
GCC 4.1 のソースコードをベースに改変を加えたものです。
オリジナルコードからの差分は、diff-from-gcc ファイルで示しております。

http://gcc.gnu.org/mirrors.html

当社の改変部分にかかる権利は、株式会社ソニー・コンピュータエンタ
テインメントに帰属します。
当社での改変を含む著作物全体が GPLv3 に基づきライセンスされており、
また個別該当ファイルについては GPLv3 第7条に従って追加された条項に
基づきライセンスされています。

----------------------------------------------------------------------
