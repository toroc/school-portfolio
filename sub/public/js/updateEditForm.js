/*Update form*/
/*Server side calls*/

document.addEventListener('DOMContentLoaded', function () {

	document.getElementById('Update').addEventListener('click', function(event) {

		var request = new XMLHttpRequest();

		var payload = {id:null, name:null, reps:null, weight:null, date:null, lbs:null};
		payload.id = document.getElementById(`id`).value;
		payload.name = document.getElementById(`name`).value;
		payload.reps = document.getElementById(`reps`).value;
		payload.weight = document.getElementById(`weight`).value;
		payload.date = document.getElementById(`date`).value;
		if (document.getElementById('lbs').checked){
			payload.lbs = 1;
		}
		else {
			payload.lbs = 0;
		}
		
		console.log(payload.lbs);
        console.log(payload);


		request.onreadystatechange = function() {
			if(request.readyState == 4 && request.status == 200) {
				window.location.href = '/public/html/workouts.html';
			}
			
		}
        
		request.open('post', '/', true);
		request.setRequestHeader('Content-Type', 'application/json');
		request.send(JSON.stringify(payload));

		event.preventDefault();
	});

	event.preventDefault();
}); 