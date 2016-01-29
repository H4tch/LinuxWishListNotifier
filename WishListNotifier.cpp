#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

typedef std::string String;

/// Todo
/// Display the Image of the Item within the Notification
/// Ability to set a budget
/// 	Items available that are more than the budget won't be displayed
/// Windows won't be supported unless I fetch the webapge myself
/// 	Use Sockets to resolve the address and GET the page

String audioFile = "notify.wav";
String amazonBuyPage = "http://www.amazon.com/gp/offer-listing/";

#ifdef WIN32
	String hideOutput = "";
	void notification( String title, String text ) {
		//String cmd = String("") + title + text;
		//system(cmd.c_str());
	}
	void playSound( String file ) {
		String cmd = String("mplay32 /play /close ") + file;
		system(cmd.c_str());
	}
#else
	String hideOutput =  "> /dev/null 2>&1";
	#ifdef __APPLE__
		void notification( String title, String text ) {
			String cmd = String("osascript -e 'display notification \"") + text + String("\" with title \"") + title + String("\"\'") + hideOutput;
			system(cmd.c_str());
		}
		void playSound( String file ) {
			String cmd = String("playsound ") + file + hideOutput;
			system(cmd.c_str());
		}
	#else // Linux/Unix
		void notification( String title, String text ) {
			String cmd = String("notify-send -t 10 -a WishList -i browser \"") + title + String("\" \"") + text + String("\"") + hideOutput;
			system(cmd.c_str());
		}
		void playSound( String file ) {
			String cmd = String("aplay ") + file + hideOutput;
			system(cmd.c_str());
		}
	#endif
#endif


class Item {
public:
	Item( String name, String id, double desiredPrice )
		: name( name ), id( id ), desiredPrice( desiredPrice )
	{}
	
	String name;
	String id;
	double desiredPrice = 0;
	
	friend std::ostream& operator<<( std::ostream& os, const Item& item ) {
		return os << item.name << " " << item.id << " " << item.desiredPrice;
	}
};


class Result {
public:
	Result( String condition, double price )
		: condition( condition ), price( price )
	{}
	String condition;
	double price = 0;
	friend std::ostream& operator<<( std::ostream& os, const Result& res ) {
		return os << res.condition << " " << res.price;
	}
};


// Print a Vector to a std::ostream
template<typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T> v ) {
	os << "[ ";
	if ( !v.empty() ) {
		for ( int i = 0; i < v.size() - 1; ++i ) { os << v[i] << ", "; }
		os << v.back() << " ";
	}
	os << "]";
	return os;
}


std::vector<Item> getItems( String filename );
std::vector<Result> getPrices( String filename );
void notifyResults( Item item, std::vector<Result> results );


int main( int argc, char* argv[] ) {
	String filename;
	if ( argc < 2 ) {
		std::cout << "USAGE " << argv[0] << " <items_filename>\n";
		return 1;
	} else {
		filename = argv[ 1 ];
	}
	
	std::ifstream temp( filename.c_str() );
	if ( !temp ) {
		std::cout << "Failed to find file: " << filename << "\n";
		//std::cout << "Attempting to parse Item" << "\n";
		return 1;
	}
	
	std::vector<Item> items = getItems( filename );
	//std::cout << "Got Items: " << items << "\n";
	
	for ( auto item : items ) {
		String download = "wget " + amazonBuyPage + item.id
							+ " -O ~" + item.id + hideOutput;
		int tries = 0;
		bool downloaded = false;
		
	  	while ( !downloaded ) {
	  		if ( tries >= 10 ) {
				std::cout << "Failed to download " << item.name
						<< "'s product page.\n";
				return 0;
	  		}
	  		++tries;
			
			int ret = system( download.c_str() );
			switch ( ret ) {
			case 0:
				downloaded = true;
				break;
			case 2048:
				//Error 503: Service Unavailable.
				continue;
			default:
				std::cout << "Error Code: " << ret << "\n";
				continue;
			}
		}
		
		std::vector<Result> results = getPrices( "~" + item.id );
		std::sort( results.begin(), results.end(),
			[]( const Result& p1, const Result& p2  ) {
				return p2.price < p1.price;
			});
		
		//std::cout << results << "\n";
		notifyResults( item, results );
		system( ("rm ~" + item.id).c_str() );
	}
	
	return 0;
}



void notifyResults( Item item, std::vector<Result> results ) {
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
	for ( auto m : matches ) { str << m.condition << "\t" << m.price << "\n"; }
	String title = item.name + " Is Available!";
	String text = str.str();
	std::cout << title << "\n" << text << amazonBuyPage << item.id << "\n\n";
	playSound( audioFile );
	notification( title, text );
}


std::vector<Item> getItems( String filename ) {
	std::ifstream file( filename.c_str() );
	String str;
	std::stringstream line;
	
	std::vector<Item> items;
	String desc;
	String id;
	String price;
	
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


void findPriceInString( String& line, String match, double& price ) {
	int pos = line.find( match );
	if ( pos != -1 ) {
		// Captures arbitrary characters representing the price. Fix eventually.
		String str = line.substr( pos + match.length() + 1, 6 );
		price = std::stod( str );
	}
}


std::vector<Result> getPrices( String filename ) {
	std::ifstream file( filename.c_str() );
	String line;
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


