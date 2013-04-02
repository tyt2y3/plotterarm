function drawer()
{
	var T=this;
	T.canx=0; T.cany=0;
	T.linestring=["","","","",""];
	T.linecolor=["#000","#F00","#0F0","#00B","#AAA"];
	T.moveTo=function(P,C)
	{
		if ( C==null) {C=4;}
		T.linestring[C] += " M "+P.x+","+P.y;
	}
	T.lineTo=function(P,C)
	{
		if ( C==null) {C=4;}
		T.linestring[C] += " L "+P.x+","+P.y;
	}
	T.drawpoint=function(P,s,C)
	{
		if (!s) {s=2;}
		if (C==null) {C=0;}
		T.moveTo({x:P.x-s, y:P.y-s},C);
		T.lineTo({x:P.x+s, y:P.y+s},C);
		T.moveTo({x:P.x-s, y:P.y+s},C);
		T.lineTo({x:P.x+s, y:P.y-s},C);
		T.moveTo({x:P.x,   y:P.y}  ,C);
	}
	T.drawpath=function(rl)
	{
		for ( var i=0; i<5; i++)
			rl.path( T.linestring[i]).attr( {stroke: T.linecolor[i], "stroke-width": 0.6});
		T.linestring=["","","","",""];
	}
	T.fillpath=function(rl,i)
	{
		rl.path( T.linestring[i]).attr( {fill: T.linecolor[i]});
		T.linestring[i]="";
	}
}
