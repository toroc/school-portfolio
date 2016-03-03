
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
app.use(bodyParser.json());

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

app.get('/search', function(req, res, next){


	var urlQs=[];

	/*Loop through the queries*/
	for (var key in req.query){
		/*Add name value pairs*/
		urlQs.push({'name' :key, 'value' :req.query[key]})
	}
	/*For debugging*/
	console.log(urlQs);
	console.log(req.query);

	/*Create array of objects*/
	var context={};
	/*Set the form type*/
	context.formType="GET";
	/*Set the list to array of params*/
	context.dataList=urlQs;
	var location=urlQs[0].value
	console.log(location);
	console.log(urlQs[0].value);
	var radius=urlQs[1].value;
	console.log(radius);
	console.log(urlQs[1].value);

	//var context = {};

	/*Request user authentication*/
	request('https://api.foursquare.com/v2/venues/search?client_id='+credentials.client_id+'&client_secret='+credentials.client_secret+'&v=20130815&near='+location+'&radius='+radius+'&limit=10', handleSearch);

	function handleSearch(err, response, body){
		if(!err && response.statusCode < 400){
			context.results=body;
			console.log(JSON.stringify(body));


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

app.get('/explore', function (req, res, next){
	res.render('explore');
});



app.get('/exploring', function(req, res){

	/*get value from explore*/
	var val = req.query.explore;

	console.log(val);

});

app.get('/formChecker', function(req,res){
	/*Create array to store form params*/
	var urlQs=[];

	/*Loop through the queries*/
	for (var key in req.query){
		/*Add name value pairs*/
		urlQs.push({'name' :key, 'value' :req.query[key]})
	}
	/*For debugging*/
	console.log(urlQs);
	console.log(req.query);

	/*Create array of objects*/
	var context={};
	/*Set the form type*/
	context.formType="GET";
	/*Set the list to array of params*/
	context.dataList=urlQs;
	/*Pass object to render page*/
	//res.render('formChecker', context);



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