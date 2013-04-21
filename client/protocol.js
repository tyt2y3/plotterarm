/** the client protocol of the plotter arm system*/

define(['states','serial'],function(states)
{
	var proto_def =
	{
		event:
		{
			entry: function(proto)
			{
				//initialize
				chrome.serial.getPorts(function(ports)
				{
					log(ports);
				});
				$('cmd1').innerHTML='connect port';
				$('cmd1').onclick=function()
				{
					proto.event('connect',$('param1').value);
					$('cmd1').innerHTML='';
					$('cmd1').onclick=null;
				}
				proto.send=function(buffer)
				{
					proto.event('send',buffer);
				}
				return 'disconnected';
			}
		},

		disconnected: //state name
		{
			event:
			{
				connect: function(proto,event,port)
				{
					var run = new Runner(
					[
						function()
						{
							chrome.serial.open(port,
							{
								bitrate: 115200
							}, run.callback);
						},
						function(con)
						{
							connectionInfo = con;
							log(con);
							chrome.serial.setControlSignals(con.connectionId,
								{
									dtr: true,
									rts: true,
									dcd: true,
									cts: true
								}, run.callback);
						},
						function()
						{
							chrome.serial.getControlSignals(connectionInfo.connectionId,
								run.callback);
						},
						function(sign)
						{
							log(sign);
							proto.event('connected');
						}
					]);
					run.callback();
				},
				connected:'connected' //go to state connected
			}
		},

		connected:
		{
			event:
			{
				entry: function(proto)
				{
					$('cmd1').innerHTML='start';
					$('cmd1').onclick = function()
					{
						$('cmd1').innerHTML='restart';
						proto.event('start');
					}
				},
				start: function()
				{
					return 'receive';
				},
				disconnect: function()
				{
					SERIAL_close(function(result)
					{
						if( result)
							log('disconnected');
					});
					return 'disconnected';
				}
			},

			receive: //a substate of connected
			{
				event:
				{
					entry: function(proto)
					{
						var This=this;
						(function periodic()
						{
							SERIAL_receive(function(received)
							{
								proto.event('received',received);
							});
							This.data.timer = setTimeout(periodic,2000);
						}());
					},
					exit: function()
					{
						clearTimeout(this.data.timer);
						this.data.timer=null;
					},
					received: function(proto,event,received)
					{
						if( proto.onreceive)
							proto.onreceive(received);
						return 'send'; //go to state send
					}
				},
				data:
				{
					timer:null
				}
			},
			send:
			{
				event:
				{
					send: function(proto,event,buffer)
					{
						SERIAL_send(buffer,function(writeinfo)
						{
							proto.event('sent',writeinfo);
						});
					},
					sent: function(proto,event,writeinfo)
					{
						log(writeinfo);
						return 'receive';
					}
				}
			}
		}
	}

	function $(id)
	{
		return document.getElementById(id)
	}
	function log(text)
	{
		if( typeof text !== 'string')
			text = JSON.stringify(text,null,'');
		$('logger').value += text+'\n';
		$('logger').scrollTop += 20;
	}
	function Runner(sequence)
	{
		var This=this;
		this.i=0;
		this.seq = sequence;
		this.callback=function()
		{
			if( This.i<This.seq.length && typeof This.seq[This.i]==='function')
			{
				This.seq[This.i].apply( null, arguments);
			}
			This.i++;
		}
	}

	var protocol = new states(proto_def);
	return protocol;
});
