document.addEventListener('DOMContentLoaded', bindButton);
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
	request.open('GET', '../../createWorkoutTable', true);
	request.send(null);
	event.preventDefault();
}

function bindButton() {
	document.getElementById('newWorkout').addEventListener('click', function(event){

		var request = new XMLHttpRequest();
		var valid = true;

		/*Save values*/
		var name = document.getElementById('name').value;
		var reps = document.getElementById('reps').value;
		var weight = document.getElementById('weight').value;
		var date = document.getElementById('date').value;
		var radios = document.getElementsByName('unit');
		var lbs;
		var lbsString= "";

		/*Ensure entire form was completed*/
		if (name == "" || reps == "" || weight == "" || date == ""){
			console.log("Form was incomplete.");
			valid = false;
		}

		/*Determine wether lbs or kilograms*/
		for (var i=0; i < radios.length; i++)
		{
			if (radios[i].checked){
				lbsString = radios[i].value;
				break;
			}
		}
		

		if (lbsString == "kilos"){
			lbs = 0;
			console.log("kilos");
		}else{
			lbs =1;
		}


		
		/*Perform based on response*/
		request.onreadystatechange = function(){

			/*Check if request received and status is OK*/
			if(request.readyState == 4 && request.status == 200){
				/*Store the response*/
				var response = JSON.parse(request.responseText);

				/**/
				for (var i=0; i < response.length; i++){
					/*save response id*/
					var responseId = response[i].id;

					/*get array of elements*/
					var ids = document.getElementsByClassName("id");
					var found = false;

					var j=0;

					/*find the id*/
					while(!found && j < ids.length){

						if (responseId == Number(ids[j].innerHTML)){
							found = true;
						}
						/*increment j*/
						j++;
					}

					/*Not found, create a row with response*/
					if (!found){
						createRow(response[i]);
					}
				}

			}
		}

		/*add the workout*/
		if (valid == true){
			/*Send  get request*/
			request.open('GET', '../../addWorkout?name=' + name + '&reps=' + reps + '&weight=' + weight + '&date=' + date + '&lbs='+lbs, true );

			/*Send null*/
			request.send(null);

			/*Clear the values*/
			document.getElementById('name').value = "";
			document.getElementById('reps').value = "";
			document.getElementById('weight').value= "";
			document.getElementById('date').value= "";
			document.getElementById('unit').value= "";
			
		}
		/*prevent default event*/
		event.preventDefault();

	});

}

function createRow(data){


	/*Create table row*/
	var newRow = document.createElement("tr");

	 /*Create the 8 data cells for the row*/ 
    for (var i=0; i < 8; i++){
        
        var newCell=document.createElement('td');
        /*Append data cell to the row*/
        newRow.appendChild(newCell);
    }


    /*Handle date formating and saving*/
	var date = data.date;

	/*Format the date*/
	if (date != "0000-00-00"){
		var formattedDate = new Date(date);
		/*Convert to JSON*/
		formattedDate = formattedDate.toJSON();
		/*Save as 10 digit format*/
		date = formattedDate.substring(0,10);
	}

	/*Handle unit and formatting */
	var lbs = data.lbs;
	
	var lbsString= "";

	if (lbs == 1){
		lbsString = "Pounds";
	}
	else{
		lbsString = "Kilograms";
	}

	console.log(lbs);


    /*Set the data cells */
    newRow.children[0].textContent = data.id;
    newRow.children[0].className = "id";
    newRow.children[1].textContent = data.name;
    newRow.children[2].textContent = data.reps;
    newRow.children[3].textContent = data.weight;
    newRow.children[4].textContent = date;
    newRow.children[5].textContent = lbsString;


	/*Create Delete Form*/
	var deleteForm = document.createElement("form");

	/*Create children*/
	var deleteFormId = document.createElement("input");
	var deleteButton = document.createElement("button");

	/*Append Children*/
	deleteForm.appendChild(deleteFormId);
	deleteForm.appendChild(deleteButton);

	/*Set the children attributes*/
	deleteForm.children[0].className = "deleteId";
	deleteForm.children[0].value = data.id;
	deleteForm.children[0].type = "hidden";

	deleteForm.children[1].textContent = "Delete";
	deleteForm.children[1].type = "submit";


	/*Append delete form to data cell*/
	newRow.children[6].appendChild(deleteForm);

	/*Get the worksout table*/
	var table = document.getElementById('workouts');

	/*Append the row to table*/
	//table.appendChild(newRow);

	/*Add button listener*/
	deleteButton.addEventListener('click', function(x){
		var id =x; 
		return function (){


			var request = new XMLHttpRequest();

			/*Perform based on response*/
			request.onreadystatechange = function(){

				/*Check if request received and status is OK*/
				if(request.readyState == 4 && request.status == 200){

					var response = JSON.parse(request.responseText);

					var table = document.getElementById('workouts');
					var rowIds = document.getElementsByClassName('id');

					/*Handle workout deletions*/
					var i =0;
					var found = false;

					while(!found && i < rowIds.length){

						if( id == Number(rowIds[i].textContent)){
							/*found the id*/
							found = true;
						}

						i++;
					}

					if(found){
						var row = rowIds[i-1].parentNode;
						table.removeChild(row);
					}
				}
			}

			/*Make request to delete the current id*/
			request.open('Get', '../../deleteWorkout?id=' + id, true);
			request.send(null);

			/*prevent default*/
			event.preventDefault();
		};

	}(deleteFormId.value));

	/*Handle edit*/
	/*Create Delete Form*/
	var editForm = document.createElement("form");
	/*Set form attributes*/
	editForm.action="/";
	editForm.method = "POST";

	/*Create children*/
	var editFormId = document.createElement("input");
	var editButton = document.createElement("input");

	/*Append Children*/
	editForm.appendChild(editFormId);
	editForm.appendChild(editButton);

	/*Set the children attributes*/
	editForm.children[0].className = "editId";
	editForm.children[0].value = data.id;
	editForm.children[0].name = "id";
	editForm.children[0].type = "hidden";

	editForm.children[1].textContent = "Edit";
	editForm.children[1].type = "submit";
	editForm.children[1].name = "Edit";
	editForm.children[1].value = "Edit";


	/*Append edit form to data cell*/
	newRow.children[7].appendChild(editForm);

	/*Append row to table*/
	table.appendChild(newRow);
}