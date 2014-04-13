
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


// Todo: Add a log file which will output the html address of available items.
// Need to remove any previous entry of it, then insert it at the bottom.
// Timestamps would be helpful too?
//
// Todo: Add ability to be notified when an item is available?

class Item
{
public:
	Item( std::string name, std::string id, double desiredPrice )
		: name( name ), id( id ), desiredPrice( desiredPrice )
	{}
	
	std::string name;
	std::string id;
	double desiredPrice = 0;
	
	friend std::ostream& operator<<( std::ostream& os, const Item& item ) {
		return os << item.name << " " << item.id << " " << item.desiredPrice;
	}
};


class Result {
public:
	Result( std::string condition, double price )
		: condition( condition ), price( price )
	{}
	std::string condition;
	double price = 0;
	friend std::ostream& operator<<( std::ostream& os, const Result& res ) {
		return os << res.condition << " " << res.price;
	}
};



template<typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T> v ) {
	os << "[ ";
	if ( !v.empty() ) {
		for ( int i = 0; i < v.size() - 1; ++i ) {
			os << v[i] << ", ";
		}
		os << v.back() << " ";
	}
	os << "]";
	return os;
}


const char* hideOutput = " > /dev/null 2>&1";

std::vector<Item> getItems( std::string filename );
std::vector<Result> getPrices( std::string filename );
void notifyResults( Item item, std::vector<Result> results );


int main( int argc, char* argv[] )
{
	std::string filename;
	if ( argc < 2 ) {
		std::cout << "USAGE " << argv[0] << " <items_file>\n";
		return 1;
	} else {
		filename = argv[ 1 ];
	}
	
	std::string amazonBuyPage = "http://www.amazon.com/gp/offer-listing/";
	
	std::vector<Item> items = getItems( filename );
	//std::cout << "Items: " << items << "\n";
	
	for ( auto item : items )
	{
		std::string download = "wget " + amazonBuyPage + item.id
							+ " -O .tmp/" + item.id + hideOutput;
		int ret = 0;
		int tries = 0;
		bool downloaded = false;
		
	  	while( !downloaded ) {
			ret = system( download.c_str() );
			if ( ret ) {
				if ( (ret == 2048) && (tries <= 10) ) {
					++tries;
					continue;
				} else {
					std::cout << "Failed to download " << item.name
							<< "'s product page.\n";
					break;
				}
			}
			downloaded = true;
		};
		
		std::vector<Result> results = getPrices( ".tmp/" + item.id );
		std::sort( results.begin(), results.end(),
			[]( const Result& p1, const Result& p2  ) {
				return p2.price < p1.price;
			});
		
		//std::cout << results << "\n";
		notifyResults( item, results );
	}
	
	return 0;
}



void notifyResults( Item item, std::vector<Result> results )
{
	std::vector<Result> matches;
	for ( auto result : results ) {
		if ( result.price && (result.price <= item.desiredPrice) ) {
			matches.push_back( result );
		}
	}
	
	if ( matches.empty() ) { return; }
	
	std::stringstream str;
	str.precision( 2 );
	str << std::fixed;
	for ( auto m : matches ) {
		str << m.condition << "  " << m.price << "\n";
	}
	std::string notify = "notify-send -t 10 -a FindPrice -i browser \""
					+ item.name + " Is Available!\" \"" + str.str() + "\"";
	system( notify.c_str() );
	std::cout << item.name << " Is Available!\n" << str.str() << "\n";
}


std::vector<Item> getItems( std::string filename )
{
	std::vector<Item> items;
	std::ifstream file( filename.c_str() );
	std::string str;
	std::stringstream line;
	std::string desc;
	std::string id;
	std::string price;
	
	while( getline( file, str ) ) {
		line.str( "" );
		line.clear();
		line.str( str );
		line >> desc >> id >> price;
		if ( line.str().empty() || (desc[0] == '#') ) { continue; }
		items.push_back( Item( desc, id, std::stod( price ) ) );
	}
	return items;
}


void findPriceInString( std::string& line, std::string match, double& price ) {
	int pos = line.find( match );
	if ( pos != -1 ) {
		std::string str = line.substr( pos + match.length() + 1, 6 );
		price = std::stod( str );
	}
}


std::vector<Result>
getPrices( std::string filename )
{
	std::ifstream file( filename.c_str() );
	std::string line;
	int lineN = 0;
	double priceNew = 0;
	double priceUsed = 0;
	double priceRefurbished = 0;
	while ( getline( file, line ) ) {
		findPriceInString( line, "New from ", priceNew );
		findPriceInString( line, "Used from ", priceUsed );
		findPriceInString( line, "Refurbished from ", priceRefurbished );
		if ( priceNew && priceUsed && priceRefurbished ) { break; }
		++lineN;
	}
	std::vector<Result> arr =
		{ Result( "New", priceNew),
		  Result( "Used", priceUsed),
		  Result( "Refurbished", priceRefurbished)
		};
	return arr;
}



