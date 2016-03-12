/*Server side database*/


/******************************************************
#   Modules for Express
******************************************************/

/*Import Node Modules*/

var express = require('express');

var handlebars = require('express-handlebars').create({defaultLayout: 'main'});

var bodyParser = require('body-parser');

var mysql = require('mysql');

var app = express();

/******************************************************
#   Setup middleware for Express
******************************************************/

/*Set up body parsing*/
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());

/*Set up default rendering and engine extension*/
app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');

/*Set port*/
app.set('port', 3000);

/*Set up static files*/
app.use('/public', express.static(__dirname + '/public'));

/*Set up pool*/
var pool = mysql.createPool({
    connectionLimit: 10,
    host        : 'localhost',
    user        : 'student',
    password    : 'default',
    database    : 'student'
});

/******************************************************
#   Set up routes
******************************************************/


/*Route for adding working*/
app.get('/addWorkout', function(req, res, next){

    /*Insert workout into the SQL table*/
    pool.query('INSERT INTO workouts (`name`, `reps`, `weight`, `date`, `lbs`) VALUES (?, ?, ?, ?, ?)', [req.query.name, req.query.reps, req.query.weight, req.query.date, req.query.lbs], function(err, result){
        if (err){
            /*Error*/
            next(err);
            /*exit function*/
            return;
        }
        
        /*Get all rows from the table*/
        pool.query('SELECT * FROM workouts', function(err, rows, fields) {
            /*Error*/
            if(err){
                next(err);
                /*exit function*/
                return;
            }
            
            /*Send all rows in table, after insering workout*/
            res.send(JSON.stringify(rows));
        });
    });
});


/*Route for creating table*/
app.get('/createWorkoutTable',function(req, res, next){

    /*Get all the rows from sql table*/
    pool.query('SELECT * FROM workouts', function(err, rows, fields) {
        /*Error*/
        if(err){
            next(err);
            /*exit function*/
            return;
        }

        /*Send all rows in table*/
        res.send(JSON.stringify(rows));
    });
});

/*Route for editing workout*/
app.get('/', function(req, res, next){

    var context = {};
    

});


/*Route for deleting workout*/



    
    /*Handle deleting data*/
    
    
    
    
    
    /*Handle editing data*/




/*Set the reset-table route as provided from assignment*/
app.get('/reset-table',function(req,res,next){
    var context = {};
    pool.query("DROP TABLE IF EXISTS workouts", function(err){ 

        var createString = "CREATE TABLE workouts("+
        "id INT PRIMARY KEY AUTO_INCREMENT,"+
        "name VARCHAR(255) NOT NULL,"+
        "reps INT,"+
        "weight INT,"+
        "date DATE,"+
        "lbs BOOLEAN)";
        pool.query(createString, function(err){
         /*Check for error*/
         if(err){
            next(err);
            /*exit function*/
            return;
         }

      });
        /*Return to the static html page*/
        res.sendFile(__dirname + 'public/html/workouts.html');
    });
});

/*Render Error Page*/
app.use(function(req,res){
  res.status(404);
  res.render('404');
});

/*Render Unknown Page*/
app.use(function(err, req, res, next){
  console.error(err.stack);
  res.status(500);
  res.render('500');
});

/*Listen on port*/
app.listen(app.get('port'), function(){
    console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});