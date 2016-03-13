// CS 290 -
// Carol D Toro
// toroc@oregonstate.edu
// Winter 2016
// Assignment: Database Interactions & UI
// Filename: updateEditForm.js
// URL: http://52.33.44.25:3001/public/html/workouts.html
//
/*Client side calls when editing*/
document.addEventListener('DOMContentLoaded', function () {

    /*Update button is clicked*/
    document.getElementById('Update').addEventListener('click', function (event) {

        var request = new XMLHttpRequest();

        /*Create payload with keys*/
        var payload = {
            id: null,
            name: null,
            reps: null,
            weight: null,
            date: null,
            lbs: null
        };

        /*Get key value pairs*/
        payload.id = document.getElementById(`id`).value;
        payload.name = document.getElementById(`name`).value;
        payload.reps = document.getElementById(`reps`).value;
        payload.weight = document.getElementById(`weight`).value;
        payload.date = document.getElementById(`date`).value;
        /*Figure out which radio was checked*/
        if (document.getElementById('lbs').checked) {
            payload.lbs = 1;
        } else {
            payload.lbs = 0;
        }

        /*Print to console for debugging*/
        console.log(payload.lbs);
        console.log(payload);

        /*Perform request based on response*/
        request.onreadystatechange = function () {

            /*Check if request received and status is OK*/
            if (request.readyState == 4 && request.status == 200) {
                /*Set the DOM window URL */
                window.location.href = '/public/html/workouts.html';
            }

        }

        /*Make a post request to the server*/
        request.open('post', '/', true);
        request.setRequestHeader('Content-Type', 'application/json');
        request.send(JSON.stringify(payload));

        /*Prevent default action from event*/
        event.preventDefault();
    });

    /*Prevent default action from event*/
    event.preventDefault();
});
