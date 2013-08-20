define(['protocol'],function(proto)
{
	//print queue management
	var queue = [];

	proto.onready=function()
	{
		if( queue.length>0)
		{
			if( queue.length>1)
				log('printing... '+queue.length+' pages remaining');
			else
				log('printing...');
			proto.event('send',serialize(queue.shift()));
		}
	}
	proto.onreceive=function(received)
	{
		log('> '+received);
	}

	function serialize(oper)
	{
		/** serialized OPER
		|  2    |   2   |   2   | bytes
		| code  |   x   |   y   | (littleEndian)
		 */
		var buf=new ArrayBuffer(oper.length*6);
		var dv=new DataView(buf);
		for( var i=0; i<oper.length; i++)
		{
			dv.setInt16(i*6,value(oper[i][0]),true);
			dv.setInt16(i*6+2,value(oper[i][1]),true);
			dv.setInt16(i*6+4,value(oper[i][2]),true);
		}
		return buf;
		function value(K)
		{
			if( typeof K==='string')
			{
				if( K.length===1)
					return K.charCodeAt(0);
				else //put [1] at high byte, [0] at low byte
					return K.charCodeAt(1)*256 + K.charCodeAt(0);
			}
			if(  K>32767) return  32767;
			if( K<-32768) return -32768;
			return K;
		}
	}
	function print(File,name)
	{
		var file=File.slice(0);
		var page_size=75;
		var num_page=1;
		while( file.length>page_size)
		{
			var page=file.splice(0,page_size);
			page.push(["^","P",0]);
			queue.push(page);
			num_page++;
		}
		file.push(["^","Q",0]);
		queue.push(file);
		if( num_page===1)
			log('file '+name+' pushed onto print queue');
		else
			log('file '+name+' ('+num_page+' pages) pushed onto print queue');
		if( proto.cur_name==='ready')
			proto.onready();
	}

	return {
		print: function(A,B)
		{
			print(A,B);
		}
	}
});
