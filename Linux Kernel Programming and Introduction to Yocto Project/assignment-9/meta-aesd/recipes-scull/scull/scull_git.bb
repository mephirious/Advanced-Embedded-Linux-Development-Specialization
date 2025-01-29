# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
#
# The following license files were not able to be identified and are
# represented as "Unknown" below, you will need to check them yourself:
#   LICENSE
LICENSE = "Unknown"
LIC_FILES_CHKSUM = "file://LICENSE;md5=f098732a73b5f6f3430472f5b094ffdb"

SRC_URI = "git://git@github.com:cu-ecen-aeld/assignment-7-arvind747.git;protocol=ssh;branch=main \
           file://0001-Modify-Makefile-to-compile-only-scull-folder.patch \
           file://scull-start-stop.sh"

# Modify these as desired
PV = "1.0+git${SRCPV}"
SRCREV = "9dd73f77eb02b3d46969ba09970095928c769d5c"

S = "${WORKDIR}/git"

inherit update-rc.d
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME:${PN} = "scull-start-stop.sh"

inherit module

FILES:${PN} += "${sysconfdir}"

EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}/scull"
EXTRA_OEMAKE += "KERNELDIR=${STAGING_KERNEL_DIR}"

do_install:append() {
	install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/scull-start-stop.sh ${D}${sysconfdir}/init.d
}