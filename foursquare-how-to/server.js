
/******************************************************
#   Setup middleware for Express
******************************************************/
/*Import Node module request*/
var request=require('request');

var path = require('path');


/*Import express node module*/
var express = require('express');
var app = express();

/*Import and setup handlebars*/
var handlebars = require('express-handlebars').create({layoutsDir: path.join(__dirname, '/app/views/layouts'), defaultLayout:'main'});

/*Import body-parser for handling POST*/
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: false }));


/*Setup default rendering engine and extension*/
app.engine('handlebars', handlebars.engine);
app.set('views', __dirname + '/app/views');
app.set('view engine', 'handlebars');


/******************************************************
#   Routes
******************************************************/

/*Render the default routes*/
app.get('/', function(req, res, next){
  res.render('index');
});


app.get('/setup', function(req, res, next){
  res.render('setup');
});

app.get('/background', function(req, res, next){
  res.render('background');
});

app.get('/user-tips', function(req, res, next){
  res.render('user-tips');
});
app.get('/user-friends', function(req, res, next){
  res.render('user-friends');
});

app.get('/user-history', function(req, res, next){
  res.render('user-history');
});

app.get('/venues-search', function(req, res, next){
  res.render('venues-search');
});

app.get('/venues-explore', function(req, res, next){
  res.render('venues-explore');
});

/*Set up port*/
app.set('port', 3001);


/*Set up static files*/
app.use(express.static(path.join(__dirname, '/app/public')));



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