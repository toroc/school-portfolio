
/******************************************************
#   Setup middleware for Express
******************************************************/
/*Import Node module request*/
var request=require('request');

/*Import express*/
var express = require('express');
var app = express();

/*Import and setup handlebars*/
var handlebars = require('express-handlebars').create({defaultLayout:'main'});

/*Import body-parser for handling POST*/
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: false }));

var oauthSignature =require('oauth-signature');

/*Setup default rendering engine and extension*/
app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');

/*Set up port*/
app.set('port', 4567);

var credentials = require('./credentials.js');

/*Set up static files*/
app.use(express.static('public'));

/******************************************************
#   Routes
******************************************************/

app.get('/', function(req, res, next){
	res.render('index');
});


app.get('/getting-started', function(req, res, next){
	res.render('getting-started');
});

app.get('/profile', function(req, res, next){
	res.render('profile');
});
app.get('/test', function(req, res, next){

	var context = {};

	/*Request user authentication*/
	request('toroc:'+credentials.user_token+'https://api.github.com/user', handleProfile)

	function handleProfile(err, response, body){

		if(!err && response.statusCode <400){
			context.profile=body;
			res.render('home', context);
		}
		else{
			console.log(response.statusCode);
		}
		next(err);
	}


});




app.use(function(req,res){
  res.status(404);
  res.render('404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.status(500);
  res.render('500');
});

app.listen(app.get('port'), function(){
  console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});