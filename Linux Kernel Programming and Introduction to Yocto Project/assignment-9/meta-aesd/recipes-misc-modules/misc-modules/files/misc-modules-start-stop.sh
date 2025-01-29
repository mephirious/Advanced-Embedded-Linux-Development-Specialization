#! /bin/sh

case $1 in
    start)
        echo "Starting misc-modules"
        insmod /lib/modules/$(uname -r)/extra/hello.ko
        module_load /lib/modules/$(uname -r)/extra/faulty
        ;;
    stop)
        echo "Stopping misc-modules"
        rmmod hello
        module_unload /lib/modules/$(uname -r)/extra/faulty
        ;;
    *)
        echo "Usage: $0 {start|stop}"
        exit 1
        ;;
esac

exit 0