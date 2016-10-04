
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
    'accessToken' : 'DQSJOGHFA2TAXFVMVRSFQNF2ZW42JADTUMP3LAAFHPGIPE4Q'
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




app.get('/tips' , function(req, res, next){
  var context = {};

  /*Build optional params*/
  var params = {};
  params.sort = "popular"

  Foursquare.Users.getTips("1065286", params,
    config.accessToken, function(error, data){
      
    if(error){
      res.send("An error was thrown: "+ error.message);
    }
    else{

      context.results = JSON.stringify(data);
      context.dataList=JSON.stringify(data.tips.items);
      console.log(data);
      renderData(data);
      res.render('tips', context);
    }
  });
  
});

function renderData(result){

  var items = JSON.parse(result);
  console.log(items);

};



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