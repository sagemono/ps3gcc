# PS3 Programmer Toolchain GCC Source

This repository preserves the GCC and GNU Binutils source packages (`toolchain-src-*`) that Sony Computer Entertainment distributed for the PlayStation 3 Programmer Tool toolchain: the PPU and SPU compilers (`ppu-lv2-gcc`, `spu-lv2-gcc`) of the Cell OS Lv-2 SDK. Each package is on its own branch, exactly as it was in its zip (file contents and Unix permissions), with nothing added; the commit message gives the zip's SHA-1.

| Branch | GCC | Binutils | Version string | Package date | Shipped compiler of the same revision |
|---|---|---|---|---|---|
| [`toolchain-src-1.8.0`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-1.8.0) | 4.0.2 | 2.16.1 | `4.0.2 (CELL 4.1.28, $Rev: 1757 $)` | 2007-06-07 | yes (Toolchain 180.002) |
| [`toolchain-src-1.9.0-GCC402`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-1.9.0-GCC402) | 4.0.2 | 2.16.1 | `4.0.2 (CELL 4.1.28, $Rev: 1849 $)` | 2007-09-05 | no (shipped: rev 1855) |
| [`toolchain-src-1.9.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-1.9.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (CELL 4.1.2.7, RTTI, $Rev: 1835 $)` | 2007-08-24 | no (shipped: rev 1830) |
| [`toolchain-src-2.2.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-2.2.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK220, $Rev: 2372 $)` | 2008-04-02 | yes (Toolchain 220.002) |
| [`toolchain-src-2.4.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-2.4.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK240, $Rev: 2499 $)` | 2008-07-09 | yes (Toolchain 240.001) |
| [`toolchain-src-2.5.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-2.5.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK250, $Rev: 2824 $)` | 2008-10-24 | yes (Toolchain 250.001) |
| [`toolchain-src-2.7.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-2.7.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK270, $Rev: 3019 $)` | 2009-04-09 | yes (Toolchain 270.001) |
| [`toolchain-src-2.8.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-2.8.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK280, $Rev: 3316 $)` | 2009-09-08 | no (shipped: rev 3189) |
| [`toolchain-src-2.8.0-GCC411-Jul29`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-2.8.0-GCC411-Jul29) | 4.1.1 | 2.17 | `4.1.1 (SDK280, $Rev: 3322 $)` | 2009-12-21 | no (shipped: rev 3189) |
| [`toolchain-src-3.0.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.0.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK300, $Rev: 3333 $)` | 2009-12-21 | no (shipped: rev 3263) |
| [`toolchain-src-3.1.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.1.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK310, $Rev: 3364 $)` | 2009-12-22 | no (shipped: rev 3320) |
| [`toolchain-src-3.3.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.3.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK330, $Rev: 3381 $)` | 2010-05-21 | no (shipped: rev 3378) |
| [`toolchain-src-3.4.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.4.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK340, $Rev: 3394 $)` | 2010-05-24 | yes (Toolchain 340.001) |
| [`toolchain-src-3.5.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.5.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK350, $Rev: 3420 $)` | 2010-12-14 | yes (Toolchain 350.001) |
| [`toolchain-src-3.6.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.6.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK360, $Rev: 3455 $)` | 2011-04-07 | yes (Toolchain 360.001) |
| [`toolchain-src-3.7.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-3.7.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK370, $Rev: 3509 $)` | 2011-08-23 | no (shipped: rev 3499) |
| [`toolchain-src-4.2.0-GCC411`](https://github.com/sagemono/ps3gcc/tree/toolchain-src-4.2.0-GCC411) | 4.1.1 | 2.17 | `4.1.1 (SDK420, $Rev: 3547 $)` | 2012-06-07 | yes (Toolchain 420.001, the last) |

The version string is `version_string` from `gcc/gcc/version.c` (GCC's base version plus SCE's `VERSUFFIX`); the package date is the newest file in the zip. The last column says whether a binary toolchain SCE shipped reports the same revision; a source package's revision is often a few commits off the shipped compiler's.

The releases of each GCC version form one history (GCC 4.0.2: 1.8.0 → 1.9.0; GCC 4.1.1: 1.9.0 → 4.2.0), so any two can be compared on GitHub, for example [3.7.0 → 4.2.0](https://github.com/sagemono/ps3gcc/compare/toolchain-src-3.7.0-GCC411...toolchain-src-4.2.0-GCC411).

## Licence

GCC and GNU Binutils are licensed under the GNU General Public License. The Sony Computer Entertainment documentation in the later packages (3.7.0 and 4.2.0) states that the copyrighted work as a whole, including Sony Computer Entertainment's modifications, is licensed based on GNU GPL version 3, subject to any applicable additional terms identified in individual files; in the earlier packages the files SCE modified or added carry the GPL notices of the code they belong to.

Original copyright notices, licence files, and attribution information have been retained.

No proprietary SDK binaries, firmware, cryptographic keys, or other material outside the original toolchain source packages are included.
