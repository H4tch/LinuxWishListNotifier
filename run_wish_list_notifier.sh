#!/bin/sh

while true; do
	./WishListNotifier MyWishList
	if (test $? -eq 1) then
		echo "Quitting..."
		exit 1
	fi
	sleep 300
done



