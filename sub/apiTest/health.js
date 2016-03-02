
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

/*Setup default rendering engine and extension*/
app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');

/*Set up port*/
app.set('port', 3000);

var credentials = require('./credentials.js');

/*Set up static files*/
app.use(express.static('public'));

/******************************************************
#   Routes
******************************************************/

app.get('/', function(req, res, next){

	var context = {};

	/*Request user authentication*/
	request('https://login.live.com/oauth20_authorize.srf?client_id='+credentials.mID+'&scope='+credentials.mScope+'&response_type=code&redirect_uri='+credentials.mRedirect, handleLogin)

	function handleLogin(err, response, body){

		if(!err && response.statusCode < 400){
			context.login = body;
			console.log(body);
			console.log(response);
			/*Render*/
			res.render('home', context);
		}else{
			console.log(err);
			if(response){
				console.log(response.statusCode);
			}
			next(err);
		}
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