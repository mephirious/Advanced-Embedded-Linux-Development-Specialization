#! /bin/sh

case $1 in
    start)
        insmod /lib/modules/$(uname -r)/extra/scull.ko
        ;;
    stop)
        rmmod scull
        ;;
    *)
        echo "Usage: $0 {start|stop}"
        exit 1
        ;;
esac

exit 0