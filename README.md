# SliTaz64_wok

This is the collection of adapted SliTaz-receipts derived from Pankso's wok for i486 SliTaz.
I quite bluntly added x86_64 to the ARCH variable and step by step patched one receipt or another to match the needs of my "bridging-wok" created on a Linux-From-Scratch (LFS) basis.
The "bridging-wok" has the single purpose of building the bridge from the actual SliTaz 32 bit packages through the LFS-based wok to a true SliTaz-only x86_64 system offering its own wok.

There are still many bugs in the receipts and I discourage everyone to use the receipts other than for the LFS-wok provided on https://people.slitaz.org/~filou/rootfs/ .
