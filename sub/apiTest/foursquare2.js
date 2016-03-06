
/******************************************************
#   Setup middleware for Express
******************************************************/
/*Import Node module request*/
var request=require('request');

/*Import express*/
var express = require('express');
var app = express();

/*import credentials*/

var config = {
  'secrets' : {
    'clientId' : '04ZLPWU4HTOHQ5EMZ4KPFJUPNIYN1ASVYXSYSWIS0MT3Z1OZ',
    'clientSecret' : '0IQVRWPOONLCIJDMIRHXTQSM0EWMNQ2M1QD31UYO4XLF1NTF',
    'redirectUrl' : 'http://localhost:3000/callback',
    'access_token' : 'DQSJOGHFA2TAXFVMVRSFQNF2ZW42JADTUMP3LAAFHPGIPE4Q'
  },
  
  'winston' : {
    'loggers': {
      'core': {
        'console': {
          'level': 'warn'
        }
      },
      'venues': {
        'console': {
          'level': 'debug'
        }
      }
    }
  }
};
/*Import foursquare module*/
var Foursquare = require('node-foursquare')(config);




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

var credentials = require('./config.js');

/*Set up static files*/
app.use(express.static('public'));

/******************************************************
#   Routes
******************************************************/

var globalURL = 'https://api.foursquare.com/v2/venues/search?client_id='+credentials.client_id+'&client_secret='+credentials.client_secret+'&v=20130815&near=';


/*Load the default routes*/

/*explore by section*/
app.get('/explore-section', function (req, res, next){
  res.render('explore-section');
});

/*search venue by query*/

app.get('/search-venues', function(req, res, next){
  res.render('search-venues');
});

/*Load the routes that will take care of receiving client side calls & making server side calls*/



/*Call venues search*/

app.get('/search' , function(req, res){
  var context = {};
  Foursquare.Venues.search({
    "lat" : "37.33",
    "lng" : "-121.8903",
    "query" : "donuts"
  }, displaySearch)
  function displaySearch(error, results){
    if (error){
      res.send("An error was thrown: "+ error.message);
    }
    else{
      context.results = results;
      console.log(results);
      res.render('/search', context);
    }
  }

});




app.get('/login', function(req, res){
  /*Call the URL with status code*/
  res.writeHead(303, { 'location': foursquare.getAuthClientRedirectUrl()});
  res.end();
});

app.get('/callback', function (req, res){
  /*Get the access token from the URL*/
  Foursquare.getAccessToken({
    code: req.query.code
  }, function (error, accessToken){
    if(error){
      res.send('An error was thrown: '+ error.message);
    }
    else{

      /*save access token for user*/
      console.log(accessToken);
    }
    
  });
});
/******************************************************
#   Helper Functions to build URLs
******************************************************/


/*Helper functions*/
function buildExploreURL(urlQueries){

  var near = urlQueries[0].value+','+urlQueries[1].value;
  console.log(near);

  var section = urlQueries[2].value;

  var limit = 15;

  var url = globalURL+near+'&section='+section+'&limit='+limit;

  console.log(url);

  return url;

};

function buildSearchURL(urlQueries){

  var near = urlQueries[0].value+','+urlQueries[1].value;
  console.log(near);

  var query= urlQueries[2].value;
  console.log(query);

  var limit = 15;

  var url = globalURL+near+'&query='+query+'&limit='+limit;

  console.log(url);

  return url;

}

function buildAuthURL(){
  var url ='https://foursquare.com/oauth2/authenticate?client_id='+credentials.client_id+'&response_type=token&redirect_uri=http://localhost:3000/get-token';
  console.log(url);
  return url;
}

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