
var config = require('./config.js');
var foursquare = require('node-foursquare')(config);


var app = express();

app.get('login', function(req, res){
	res.writeHead(303, { 'location': foursquare.getAuthClientRedirectUrl()});
	res.end();
});

app.get('/callback', function (req, res){
	foursquare.getAccessToken({
		code: req.query.code
	}, function (error, accessToken){
		if(error){
			res.send('An error was thrown: '+ error.message);
		}
		else{
			/*save access token and redirect*/
			console.log(accessToken);
		}
		
	});
});