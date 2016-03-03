


$(function(){
	$('#explore').on('keyup', function(e){

		if(e.keyCode === 13){

			var parameters = { explore: $(this).val() };
				$.get('/exploring', parameters, function(data){
					$('#results').html(data);
				});
			};
			
	});
});