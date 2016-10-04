var foursquare= (require('foursquarevenues'))('CLIENTIDKEY', 'CLIENTSECRETKEY');

var params = {
	"near": "New York City, NY"
};

foursquare.getVenues(params, function(error, venues){
	if(!error){
		console.log(venues);
	}
});

foursquare.exploreVenues(params, function(error, venues){
	if (!error){
		console.log(venues);
	}
});