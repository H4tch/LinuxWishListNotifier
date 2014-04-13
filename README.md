LinuxWishListNotifier
=====================

C++ Linux "script" that notifies you when any of your specified Amazon items falls below a certain price point. Does not scrape your Amazon's WishList.


### Requirements
	Linux
	wget
	notify-send


## How to
Go to amazon.com. Find a product. Copy the id of the product. It's usually
after "dp/" in the URL. For example: "amazon.com..../dp/##########".

For some products, like books, different buying options have different ids. For
books, there are the options: paperback/hardcover. In this case you need to
visit the buying options page. At the top and in n the middle of the product
page there are buying options. Click on the price that is in the category you
want. At this page the id you need is usually after "offer-listing/" in the
URL.


#### To test the program run:
	make
	./WishList Items

You can run "./run_wish_list.sh" to automatically run the program. It checks every 5min
(300 seconds) by default.


#### Notes
You cannot currently check for certain item conditions (new, used,
refurbished). You could however, remove the functionality from the getPrices()
function to avoid certain checks. However, this would effect all items.

