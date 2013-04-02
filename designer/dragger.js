function dragger(TP,div_id)
{
	this.drag=-1;
	this.TP=TP;
	this.disable=false;
	this.child=null;

	this.onMouseDown = function(e)
	{
		if (e.token=='in') {
			this.parent=e.parent;
		} else {
			var T=this.parent;
			e=e?e:event;
			T.xmouse=e.clientX-T.canx+document.body.scrollLeft;
			T.ymouse=e.clientY-T.cany+document.body.scrollTop;
			T.lastxmouse=T.xmouse;
			T.lastymouse=T.ymouse;

			if ( !T.disable)
			{
				var TP=T.TP;

				T.drag=[];
				var thres = 8;
				for ( var i=TP.P.length-1; i>=0; i--)
				{
					if( T.drag.length > 0)
					{
						if( TP.P[i].x === TP.P[T.drag[0]].x && TP.P[i].y === TP.P[T.drag[0]].y)
							T.drag.push(i);
					}
					else
					{
						if( Math.abs(T.xmouse - TP.P[i].x) < thres && Math.abs(T.ymouse - TP.P[i].y) < thres)
							T.drag.push(i);
					}
				}
				if( T.drag.length===0)
					T.drag=-1;
			}

			if( T.child)
			{
				if ((e.which && e.which == 3) || (e.button && e.button == 2))
					T.child('rightmousedown',T.xmouse,T.ymouse);
				T.child('mousedown',T.xmouse,T.ymouse);
			}
		}
	}

	this.onMouseUp = function(e)
	{
		if (e.token=='in') {
			this.parent=e.parent;
		} else {
			var T=this.parent;
			var TP=T.TP;

			if( T.drag !== -1)
			{
				app1.change();
				T.drag=-1;
			}
		}
	}
	this.onMouseMove = function(e)
	{
		if (e.token=='in') {
			this.parent=e.parent;
		} else {
			var T=this.parent;
			var TP=T.TP;

			e=e?e:event;
			T.xmouse=e.clientX-T.canx+document.body.scrollLeft;
			T.ymouse=e.clientY-T.cany+document.body.scrollTop;
			if ( T.drag!==-1)
			{
				for( var j=0; j<T.drag.length; j++)
				{
					var i = T.drag[j];
					TP.P[i].x = T.xmouse;
					TP.P[i].y = T.ymouse;
				}
				TP.redraw();
			}
			T.lastxmouse=T.xmouse;
			T.lastymouse=T.ymouse;
		}
	}
	this.printpoints = function(TP)
	{
		xstr = "this.TP.P.x=[";
		ystr = "this.TP.P.y=[";
		for ( var i=0; i<TP.px.length; i++)
		{
			if ( i != 0)
			{
				xstr+=",";
				ystr+=",";
			}
			xstr += TP.P[i].x;
			ystr += TP.P[i].y;
		}
		xstr += "];";
		ystr += "];";
		alert(xstr+"\n"+ystr);
	}

	tardiv = document.getElementById(div_id);

	function getPositionLeft(This){
		var el=This;var pL=0;
		while(el){pL+=el.offsetLeft;el=el.offsetParent;}
		return pL
	}
	function getPositionTop(This){
		var el=This;var pT=0;
		while(el){pT+=el.offsetTop;el=el.offsetParent;}
		return pT
	}
	this.canx=getPositionLeft(tardiv);
	this.cany=getPositionTop(tardiv);

	tardiv.onmousedown	=this.onMouseDown;
	tardiv.onmouseup	=this.onMouseUp;
	tardiv.onmousemove	=this.onMouseMove;

	tardiv.onmousedown({token:'in', parent:this});
	tardiv.onmouseup({token:'in', parent:this});
	tardiv.onmousemove({token:'in', parent:this});
}
