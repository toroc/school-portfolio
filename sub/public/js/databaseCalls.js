//document.addEventListener('DOMContentLoaded', bindButtons);
document.addEventListener('DOMContentLoaded', getTable);

function getTable() {
	var request = new XMLHttpRequest();

	/*Perform based on response*/
	request.onreadystatechange = function(){
		/*Check if request received and status is OK*/
		if(request.readyState == 4 && request.status == 200){
			/*Store the response*/
			var response = JSON.parse(request.responseText);

			/*Create row for each response*/
			for (var i=0; i < response.length; i++){
				createRow(response[i]);
			}
		}
	}
	/*Perform get request*/
	request.open('GET', 'createWorkoutTable', true);
	request.send(null);
	event.preventDefault();
}

