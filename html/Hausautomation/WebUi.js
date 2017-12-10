  var endpoints;
  var selectedEndpointIndex = 0;
  var initd = false;
  var internalWsUri = "ws://192.168.178.2:3001";
  var externalWsUri = "ws://casa-amarilla.selfhost.eu:3001";
  var websocket = null;
  
  
  
  var debugTextArea = document.getElementById("debugTextArea");
	function debug(message) {
		//debugTextArea.value += message + "\n";
		//debugTextArea.scrollTop = debugTextArea.scrollHeight;
    }
	function initWebSocket(wsUri) {
        try {
			if (typeof MozWebSocket == 'function')
				WebSocket = MozWebSocket;
				websocket = new WebSocket( wsUri );
                websocket.onopen = function (evt) {
					debug("CONNECTED");
					setStateMessage( "Server verbunden", "w3-text-green");
					document.getElementById("reconnectButton").style ="visibility: hidden";
					};
				websocket.onmessage = function (evt) {
					console.log( "Message received :", evt.data );
                    debug( evt.data );
					parseMessage(evt.data);
                };
				websocket.onclose = function (evt) {
                        debug("DISCONNECTED");	
						setStateMessage( "Verbindung getrennt &nbsp; &nbsp; &nbsp;", "w3-text-red");
						document.getElementById("reconnectButton").style ="visibility: visible";
						
                };
				websocket.onerror = function (evt) {
						setStateMessage( "Verbindung getrennt &nbsp; &nbsp; &nbsp;", "w3-text-red");
                        debug('ERROR: ' + evt.data);
						if(wsUri == internalWsUri) {
							//make a second attemp with external domain name
							initWebSocket(externalWsUri);
						}
                };
		}catch (exception) {
				debug('ERROR: ' + exception);
        }
	}
	function requestState(requestedState, number) {
		var request = {mac: endpoints[number].mac, state: requestedState}
		var requestMessage = JSON.stringify(request);
		websocket.send(requestMessage);

	}
	function setStateMessage(message, color) {
		document.getElementById("statusMessage").innerHTML = "<b>";
		document.getElementById("statusMessage").innerHTML += message;
		document.getElementById("statusMessage").innerHTML += "</b>";
		document.getElementById("statusMessage").className = color;
	}
	function reconnect() {		
		setStateMessage("Verbinden mit Server &nbsp; &nbsp; &nbsp;", "w3-text-indigo");
		initWebSocket(internalWsUri)
		}
	function selectEndpoint(index) {
		selectedEndpointIndex = index;
		var i;
		for(i in endpoints) {
			document.getElementById("endpoint" + i).className = (i == selectedEndpointIndex) ? "endpointScreen-visible" : "endpointScreen-hidden";
		}
		w3_close();
		//document.getElementById("statusMessage").innerHTML = endpoints[index].alias;
		//document.getElementById("statusMessage").innerHTML += "&nbsp; &nbsp; &nbsp";
		//document.getElementById("statusMessage").innerHTML += endpoints[index].state ? "EIN" : "Aus";
		}

