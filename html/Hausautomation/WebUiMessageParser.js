function parseMessage(message) {
	var data = JSON.parse(message);
	endpoints = data.endpoints;
		
	createNav();
	createEndpointScreens();
}
function createNav() {
	//write Navigation
	document.getElementById("endpointsNav").innerHTML = "";
	var i = 0;
	for(i in endpoints) {
		document.getElementById("endpointsNav").innerHTML += "<a href='#' onclick='selectEndpoint(" + i +")'>" + endpoints[i].alias  + "</a>";
	}
}

function createEndpointScreens() {
	document.getElementById("main").innerHTML = "";
	var i;
	for(i in endpoints) {
		document.getElementById("main").innerHTML +=  	"<div class='endpointScreen" + (i == selectedEndpointIndex ? "-visible'" : "-hidden'") +
														"id='endpoint" + i + "'><h2>" +
														endpoints[i].alias + "&nbsp; &nbsp; &nbsp" +
														"<span style='color:" + (endpoints[i].state ? "yellow" : "black") + "'>" +
														(endpoints[i].state ? "EIN" : "AUS") + "</span></h2>" +
														"<hr>" +
														"<p><a href='#' id='onSwitch' onClick='requestState(true," + i + " )'>einschalten </a> &nbsp; &nbsp; &nbsp" +
														"<a href='#'  id='offSwitch' onClick='requestState(false," + i + " )'>ausschalten</a>" +
														"<p><input type='checkbox' name='auto-state' value='Automatik' onChange=''>Automatik</p> " +
														"<p><h3>Zeitpläne</h3>" +
														"<ul><li></li><li></li><li>...</li></ul>"
														"</div>";
	}

}