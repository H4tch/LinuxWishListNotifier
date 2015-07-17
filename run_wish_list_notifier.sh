#!/bin/sh

# Must be run with an absoltue path, or relative to LinuxWishListNotifier dir.
if ( test $# -eq 2 ); then WISHLIST=$1
else WISHLIST="MyWishList"; fi
cat $WISHLIST
if ( test $? -ne 0 ); then exit 1; fi

while true; do
	cd ~/Projects/WishListNotifier/
	./WishListNotifier $WISHLIST
	if (test $? -eq 1) then
		echo "Quitting..."
		exit 1
	fi
	sleep 300
done



