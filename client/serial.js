var connectionInfo;

function SERIAL_send(str,callback)
{
	if( typeof str==='string')
	{	//str is a javascript string
		var buf=new ArrayBuffer(str.length+4);
		var dv=new DataView(buf);
		dv.setInt32(0,str.length,true);
		var bufView=new Uint8Array(buf);
		for (var i=0; i<str.length; i++)
			bufView[i+4]=str.charCodeAt(i);

		chrome.serial.write(connectionInfo.connectionId,
			buf,
			function(writeInfo)
			{
				if( callback)
					callback(writeInfo);
			});
	}
	else if( str instanceof ArrayBuffer)
	{	//str is an ArrayBuffer
		var size=new ArrayBuffer(4);
		var dv=new DataView(size);
		dv.setInt32(0,str.byteLength,true);
		chrome.serial.write(connectionInfo.connectionId,
			size,
			function(writeInfo)
			{
				if( writeInfo.bytesWritten===4)
				{
					chrome.serial.write(connectionInfo.connectionId,
						str,
						function(writeInfo)
						{
							if( callback)
								callback(writeInfo);
						});
				}
				else
				{
					if( callback)
						callback({bytesWritten:0,SERIAL_error:'failed'});
				}
			});
	}
}
function SERIAL_receive(callback)
{
	var head=new ArrayBuffer(4),
		dataread='',
		total=null,
		count=0;
	function readhead(readInfo)
	{
		if (readInfo && readInfo.bytesRead>0 && readInfo.data)
		{
			var dv=new DataView(head);
			var readbuffer=new Uint8Array(readInfo.data);
			for( var i=0; i<readInfo.bytesRead; i++)
				dv.setUint8(count+i,readbuffer[i],true);
			count+=readInfo.bytesRead;
			if( count>=4)
			{
				total = dv.getInt32(0,true); //littleEndian int
				if( total>0 && total<1000000)
				{
					count = 0;
					chrome.serial.read(connectionInfo.connectionId, total, readcontent);
				}
			}
		}
	}
	function readcontent(readInfo)
	{
		if (readInfo && readInfo.bytesRead>0 && readInfo.data)
		{
			count+=readInfo.bytesRead;
			var str=ab2str(readInfo.data);
			dataread+=str;
			if( count>=total)
			{
				if( callback)
					callback(dataread);
				SERIAL_flush();
			}
		}
	}
	function ab2str(buf)
	{
		return String.fromCharCode.apply(null, new Uint8Array(buf));
	}
	chrome.serial.read(connectionInfo.connectionId, 4, readhead);
}
function SERIAL_flush(callback)
{
	chrome.serial.flush(connectionInfo.connectionId, callback?callback:function(){});
}
function SERIAL_close(callback)
{
	chrome.serial.close(connectionInfo.connectionId, callback?callback:function(){});
}

/*
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

