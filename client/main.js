requirejs(['protocol'],function(proto)
{

proto.onreceive=function(received)
{
	log('> '+received);
}
$('cmd2').innerHTML='send';
$('cmd2').onclick=function()
{
	var oper;
	try {
		oper = JSON.parse($('sender').value);
	} catch (e) {
		log(e.message);
		return;
	}
	if( oper[oper.length-1][0]!=='Q')
		oper.push(["^","Q",0]);
	oper.unshift(["^", "H".charCodeAt(0), oper.length]);
	if( typeof oper!==undefined)
	{
		proto.send(serialize(oper));
	}
}
function serialize(oper)
{
	/** serialized OPER
	| 1  |   2   |   2   | bytes
	|code|   x   |   y   | (littleEndian)
	 */
	var buf=new ArrayBuffer(oper.length*5);
	var dv=new DataView(buf);
	for( var i=0; i<oper.length; i++)
	{
		dv.setUint8(i*5,value(oper[i][0]),true);
		dv.setInt16(i*5+1,value(oper[i][1]),true);
		dv.setInt16(i*5+3,value(oper[i][2]),true);
	}
	return buf;
	function value(K)
	{
		if( typeof K==='string')
		{
			if( K.length===1)
				return K.charCodeAt(0);
			else //put [1] at high byte, [0] at low byte
				return K.charCodeAt(1)*2*2*2*2*2*2*2*2 + K.charCodeAt(0);
		}
		return K;
	}
}

//helpers
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

/*
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
function test()
{
	var run = new Runner(
	[
		function()
		{
			chrome.serial.getPorts(run.callback);
		},
		function(ports)
		{
			log(ports);
			chrome.serial.open("COM4",
			{
				bitrate: 115200
			}, run.callback);
		},
		function(con)
		{
			connectionInfo = con;
			log(connectionInfo);
			chrome.serial.setControlSignals(connectionInfo.connectionId,
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
			$('cmd1').innerHTML='shake hand';
			$('cmd1').onclick = function()
			{
				SERIAL_receive(function(received)
				{
					log(received);
					SERIAL_send('hello from chrome',function(writeInfo)
					{
						log(writeInfo);
					});
				});
			}
		}
	]);
	run.callback();
}
function test2()
{
	chrome.serial.getPorts(function(ports)
	{
		log(ports);
	});
	$('cmd1').innerHTML='connect port';
	$('cmd1').onclick=function()
	{
		var run = new Runner(
		[
			function()
			{
				chrome.serial.open($('param1').value,
				{
					bitrate: 115200
				}, run.callback);
			},
			function(con)
			{
				connectionInfo = con;
				log(connectionInfo);
				chrome.serial.setControlSignals(connectionInfo.connectionId,
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
				$('cmd1').innerHTML='run';
				$('cmd1').onclick = function()
				{
					var loop = new Runner(
					[
						function()
						{
							SERIAL_receive(loop.callback);
						},
						function(received)
						{
							log(received);
							SERIAL_send('hello from chrome',loop.callback);
						},
						function()
						{
							loop.i=-1;
							setTimeout(loop.callback,2000);
						}
					]);
					loop.callback();
				}
			}
		]);
		run.callback();
	}
} */

});
