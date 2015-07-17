# 	WishListNotifier
---------------------
C++ Linux/Mac utility that notifies you when any of your specified Amazon items
fall below a certain price point.


### Linux Requirements
* `wget` - for downloading the Amazon Product Page
* `aplay` - Optioanl, plays a sound when a match is found
* `notify-send` - Optional, displays a desktop notfication


### Mac Requirements
* `wget` - for downloading the Amazon Product Page
* `afplay` - Optional, plays a sound when a match is found
* `display notification` - Optional, displays a desktop notfication


## How to
Go to `www.amazon.com` and find a product.
Copy the id of the product. It's usually after "dp/" in the URL.
For example: "amazon.com..../dp/##########".
**Add your products to [MyWishList file](/MyWishList)**

For some products, like books, different buying options have different ids.
Example: paperback or hardcover.
In this case you need to visit the buying options page to get the product ID.

In the top center of the page there are buying options.
Click on the price that is next to the category you want.
On this page the Id you need is usually after "offer-listing/" in the URL.


#### Build and run the program:
> make
> ./WishListNotifier MyWishList

The `run_wish_list_notifier.sh` script will periodically run the utility in the
background.
> ./run_wish_list_notifier.sh MyWishList


#### Notes
Currently, you can't specify item conditions (new, used, refurbished).
You could however, customize the `getPrices()` function within
`WishListNotifier.cpp`.


#### Todo
* Add Monoprice support
	* [wish list](https://www.monoprice.com/MyAccount/MyWishList)


[![MIT LICENSE Badge](https://img.shields.io/github/license/mashape/apistatus.svg)](/LICENSE.md "MIT Licensed")

