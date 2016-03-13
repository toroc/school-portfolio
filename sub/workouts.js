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


/*Route for adding workout*/
app.get('/addWorkout', function(req, res, next){

     console.log("Debug inside /addWorkout");
    
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
app.post('/', function(req, res, next){
    
    console.log("inside edit at beggining");

    /*Edit button was selected*/
    if (req.body['Edit']){
        
        console.log("before makign query request");

        /*Select row matching id*/
        pool.query('SELECT * FROM workouts WHERE id=(?)', [req.body.id], function (err, rows, fields){

            /*Error*/
            if(err){
                next(err);
                console.log("error");
                /*exit function*/
                return;
            }

            /*store data from row fields*/
            var data = rows[0];

            /*Add additional fields to correspond to template*/
            data.kilo;

            /*data is in kilo*/
            if (data.lbs == 0){
                data.kilo = 1;
            }
            else{
                /*data is in lbs*/
                data.kilo = 0;
            }

            /*Reformat the data which was parsed on the client side*/
            if(data.date != "0000-00-00"){
                /*Create new data and store */
                var date = new Date(rows[0].date);

                /*Convert to JSON*/
                date = date.toJSON();

                /*Store 10 digits*/
                data.date = date.substring(0,10);
            }

            console.log(data);
            
            /*Render the server side page with the current values*/
            res.render('editWorkout', data);
        });
    }
    else{
        
        /**/
        console.log("before selecting updated row");

        /*Row has been updated*/
        pool.query('SELECT * FROM workouts WHERE id=(?)', [req.body.id], function(err, row){

            /*Error*/
            if(err){
                next(err);
                return;
            }
            
            console.log("before row length");
            console.log(row);
            console.log(row.length);
            if (row.length ==1){
                
                var currentRow = row[0];

                /*Update Values only IF different than current */
                console.log("before trying to update values");

                pool.query('UPDATE workouts SET name=?, reps=?, weight=?, date=?, lbs=? WHERE id=?', [req.body.name || currentRow.name, req.body.reps || currentRow.reps, req.body.weight || currentRow.weight, req.body.date || currentRow.date, req.body.lbs, req.body.id], function(err, rows, results){

                    var data = {};
                    /*Error*/
                    if(err){
                        next(err);
                        /*exit fn*/
                        return;
                    }

                    /*Return to the static html page*/
                    res.sendFile(__dirname + '/public/html/workouts.html');
                });
            }
        });

    }
    

});


/*Route for deleting workout*/
app.get('/deleteWorkout', function(req, res, next){

    console.log("Debug inside /deleteWorkout");
    /*Delete row from sql with the corresponding id*/

    pool.query('DELETE FROM workouts WHERE id=(?)', [req.query.id], function(err, result){
        /*Error*/
        if(err){
            next(err);
            /*exit function*/
            return;
        }
        /*Send result from query request to client*/
        res.send(JSON.stringify(result));
    });
});




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
        res.sendFile(__dirname + '/public/html/workouts.html');
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