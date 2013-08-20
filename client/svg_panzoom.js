define(function()
{
	function svg_panzoom(el)
	{
		return new panzoom(el);
	}
	function panzoom(svgDocument)
	{
		var box,obox,doc,lastxmouse,lastymouse,dragging,aspect;
		function mousewheel(e)
		{
			if (!e) e = window.event;
			var d = e.detail ? -e.detail:e.wheelDelta/120;
			var pad = 5;
			if( e.stopPropagation)
			{
				e.stopPropagation();
				e.preventDefault();
			}
			zoominx(d*pad);
		}
		function mousedown(e)
		{
			if (!e) e = window.event;
			lastxmouse=e.clientX;
			lastymouse=e.clientY;
			dragging=true;
		}
		function mouseup()
		{
			dragging=false;
		}
		function mousemove(e)
		{
			if( dragging)
			{
				if (!e) e = window.event;
				var xmouse=e.clientX,
					ymouse=e.clientY,
					dx = xmouse-lastxmouse,
					dy = ymouse-lastymouse;
				lastxmouse=xmouse;
				lastymouse=ymouse;
				pan( dx,dy);
			}
		}
		function pan(x,y)
		{
			var rw = box.width/doc.parentNode.clientWidth,
				rh = box.height/doc.parentNode.clientHeight,
				rr = rw>rh?rw:rh;
			box.x -= x*rr;
			box.y -= y*rr;
			doc.setAttribute('viewBox', [box.x,box.y,box.width,box.height].join(' '));
		}
		function zoominx(x)
		{
			if( aspect < 1)
			{
				var y = x*aspect;
			}
			else
			{
				var y=x;
				x=y/aspect;
			}
			box.x += x;
			box.y += y;
			box.width  += -2*x;
			box.height += -2*y;
			doc.setAttribute('viewBox', [box.x,box.y,box.width,box.height].join(' '));
		}
		function zoom(r)
		{
			box.x = obox.x;
			box.y = obox.y;
			box.width  = obox.width;
			box.height = obox.height;
			zoominx((r-1)*obox.width*0.5);
		}
		//init
		doc = svgDocument;
		doc.setAttribute('width','100%');
		doc.setAttribute('height','100%');
		box = doc.getBBox();
		box = { x:box.x,y:box.y,width:box.width,height:box.height};
		obox = { x:box.x,y:box.y,width:box.width,height:box.height}; //original box
		aspect = obox.height/obox.width;
		doc.addEventListener('mousewheel', mousewheel, false);
		doc.addEventListener('mousemove', mousemove, false);
		doc.addEventListener('mousedown', mousedown, false);
		doc.addEventListener('mouseup', mouseup, false);
		zoominx(-10);
		//public
		this.pan=pan;
		this.zoom=zoom;
	}

	return svg_panzoom;
});
