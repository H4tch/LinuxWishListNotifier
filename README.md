WishListNotifier
=====================

C++ Linux/Mac utility that notifies you when any of your specified Amazon items
fall below a certain price point.


### Linux Requirements
	wget - for downloading the Amazon Product Page
	aplay - Optioanl, plays a sound when a match is found
	notify-send - Optional, displays a desktop notfication

### Mac Requirements
	wget - for downloading the Amazon Product Page
	afplay - Optional, plays a sound when a match is found
	display notification - Optional, displays a desktop notfication


## How to
Go to www.amazon.com and find a product.
Copy the id of the product. It's usually after "dp/" in the URL.
For example: "amazon.com..../dp/##########".
**Add your products to MyWishList file**

For some products, like books, different buying options have different ids.
For books, there are the options: paperback or hardcover.
In this case you need to visit the buying options page.

	At the top and in the center of the page there are buying options
	Click on the price that is next to the category you want
	At this page the id you need is usually after "offer-listing/" in the URL


#### Build and run the program:
	make
	./run_wish_list_notifier.sh MyWishList

"./WishListNotifier MyWishList" will only run the utility once


#### Notes
You cannot currently specify certain item conditions (new, used, refurbished).
You could however, modify the getPrices() function to avoid certain checks.


