# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
#
# The following license files were not able to be identified and are
# represented as "Unknown" below, you will need to check them yourself:
#   LICENSE
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "git://git@github.com/cu-ecen-aeld/assignments-3-and-later-arvind747.git;protocol=ssh;branch=main \
           file://aesdchar-start-stop.sh"

# Modify these as desired
PV = "1.0+git${SRCPV}"
SRCREV = "f0344abf522f14d445e2c1338263d765ebbabaed"

S = "${WORKDIR}/git/aesd-char-driver"

inherit update-rc.d
INITSCRIPT_PACKAGES = "${PN}"
INITSCRIPT_NAME:${PN} = "aesdchar-start-stop.sh"

inherit module

FILES:${PN} += "${sysconfdir}"
FILES:${PN} += "${bindir}/aesdchar_load"
FILES:${PN} += "${bindir}/aesdchar_unload"

EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}"
EXTRA_OEMAKE += "KERNELDIR=${STAGING_KERNEL_DIR}"

do_install:append() {
	install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/aesdchar-start-stop.sh ${D}${sysconfdir}/init.d

	install -d ${D}${bindir}
	install -m 0755 ${S}/aesdchar_load ${D}${bindir}/
	install -m 0755 ${S}/aesdchar_unload ${D}${bindir}/
}
