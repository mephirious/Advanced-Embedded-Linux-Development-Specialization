#! /bin/sh

case $1 in
    start)
        echo "aesdchar_load executed"
        aesdchar_load
        ;;
    stop)
        echo "aesdchar_unload executed"
        aesdchar_unload
        ;;
    *)
        echo "Usage: $0 {start|stop}"
        exit 1
        ;;
esac

exit 0