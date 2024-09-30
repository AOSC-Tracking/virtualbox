#!/usr/bin/bash
# AOSC OS script for local debugging

# autobuild stubs

SRCDIR=.
OUTPUTDIR="$SRCDIR"/out/linux.amd64/release/bin
PKGDIR=""

abinfo() {
    echo "--------- " "$@"
}

if [[ "$(id -u)" != 0 ]]; then
    abinfo "Switching to root"
    exec sudo "$0" "$@"
fi

# from abbs build script

abinfo "Preparing target directories ..."
install -vdm755 "$PKGDIR"/usr/{src,bin,lib/virtualbox}

abinfo "Installing binaries ..."
install -vm755 "$OUTPUTDIR"/VBox.sh "$PKGDIR"/usr/bin/VBox
for i in VBoxHeadless VBoxManage VBoxSDL VirtualBox vboxwebsrv VBoxBalloonCtrl; do
    ln -svf VBox "$PKGDIR"/usr/bin/$i
    ln -svf VBox "$PKGDIR"/usr/bin/${i,,}
done

install -vm755 "$OUTPUTDIR"/vboximg-mount "$PKGDIR"/usr/bin

abinfo "Installing libraries ..."
install -vm755 "$OUTPUTDIR"/*.so "$PKGDIR"/usr/lib/virtualbox
install -vm644 "$OUTPUTDIR"/*.r0 "$OUTPUTDIR"/VBoxEFI*.fd "$PKGDIR"/usr/lib/virtualbox

abinfo "** (1/2) Installing setuid root binaries ..."
for i in VBoxSDL VBoxHeadless VBoxNetDHCP VBoxNetAdpCtl VBoxNetNAT VirtualBoxVM; do
    install -vm4755 "$OUTPUTDIR/$i" -t "$PKGDIR"/usr/lib/virtualbox
done

abinfo "** (2/2) Installing other binaries ..."
for i in VBoxManage VirtualBox VBoxSVC VBoxExtPackHelperApp VBoxXPCOMIPCD.so VBoxBalloonCtrl vboxwebsrv webtest; do
    install -vm755 "$OUTPUTDIR/$i" -t "$PKGDIR"/usr/lib/virtualbox
done

abinfo "Installing components ..."
install -vdm755 "$PKGDIR"/usr/lib/virtualbox/components
install -vm755 "$OUTPUTDIR"/components/* -t "$PKGDIR"/usr/lib/virtualbox/components
