//global helpers
function $(id)
{
	return document.getElementById(id)
}
function log(text)
{
	if( typeof text !== 'string')
		text = JSON.stringify(text,null,'');
	$('logger').value += text+'\n';
	$('logger').scrollTop += 100;
}
function llog(text)
{
	if( typeof text !== 'string')
		text = JSON.stringify(text,null,'');
	$('logger').value += text;
}

requirejs(['printer','path_parser','svg_panzoom','ui'],function(printer,PathParser,svg_panzoom)
{

$('cmd2').innerHTML='send';
$('cmd2').onclick=function()
{
	var oper;
	try {
		oper = JSON.parse($('sender').value);
		printer.print(oper,'untitled');
	} catch (e) {
		log(e.message);
		return;
	}
};

(function()
{
//fit to paper
$('effective_area').style.display='none';
var box = $('svg_ref').getBBox();
$('svg_ref').setAttribute('viewBox', [box.x,box.y,box.width,box.height].join(' '));
$('effective_area').style.display='';
}());

//svg file load
var svgdoc;
function load_svg(file)
{
	var reader = new FileReader();
	reader.onload = function(e)
	{
		var txt = e.target.result;
		txt = txt.slice(txt.indexOf('<svg'));
		$('svg_inner_1').innerHTML = txt;
		svgdoc = $('svg_inner_1').firstChild; //the inline svg node
		svgdoc.filename = file.name;
		svg_panzoom(svgdoc);
	}
	reader.readAsText(file);
}
function handleFile(files)
{
	load_svg(files[0]);
}
function handleFileSelect(e)
{
	var files = e.target.files;
	handleFile(files);
}
function handleDragOver(e)
{
	e.stopPropagation();
	e.preventDefault();
	e.dataTransfer.dropEffect = 'copy';
}
function handleFileDrop(e)
{
	e.stopPropagation();
	e.preventDefault();
	var files = e.dataTransfer.files;
	handleFile(files);
}
function getOffsetLeftTop(This)
{
	var el=This;
	var p={x:0,y:0};
	while (el)
	{
		p.x += el.offsetLeft;
		p.y += el.offsetTop;
		el = el.offsetParent;
	}
	return p;
}
$('fileselect').addEventListener('change', handleFileSelect, false);
$('page1').addEventListener('dragover', handleDragOver, false);
$('page1').addEventListener('drop', handleFileDrop, false);
//svg file parse
$('svg_print').onclick=function()
{
	if( !svgdoc) return;
	var output = [],
		par = getOffsetLeftTop(svgdoc), //parent x,y,w,h
		pr = {w:3000,h:2500}, //paper width and height
		paths = svgdoc.getElementsByTagName('path'),
		ctm = svgdoc.getScreenCTM(), //svg to screen space transformation matrix
		pathm, //element to svg space transformation matrix
		po, //element space origin
		refm = $('svg_ref').getScreenCTM().inverse(), //reference svg space
		pprm = $('paperA4').getTransformToElement($('svg_ref')).inverse(); //paper space
	par.w = svgdoc.parentNode.clientWidth;
	par.h = svgdoc.parentNode.clientHeight;
	if( paths)
	for( var i=0; i<paths.length; i++)
	{
		pathm = paths[i].getTransformToElement(svgdoc);
		po = trans(0,0);
		var parser = new PathParser();
		parser.parsePath(paths[i]);
		var res = parser.result;
		if( res[0][0]==='m')
			res[0][0] = 'M'; //the first move command of each path must be absolute;
		var lastpos = {x:0,y:0};
		var lastM = {x:0,y:0};
		for( var j=0; j<res.length; j++)
		{
			var cmd = res[j];
			switch (cmd[0])
			{
				case 'M':
				case 'm':
				case 'L':
				case 'l':
					oper(cmd[0],cmd[1],cmd[2]);
				break;
				case 'Z':
				case 'z':
					oper('L',lastM.x,lastM.y);
				break;
				case 'H':
					oper('L',cmd[1],lastpos.y);
				break;
				case 'V':
					oper('L',lastpos.x,cmd[1]);
				break;
				case 'h':
					oper('l',cmd[1],0);
				break;
				case 'v':
					oper('l',0,cmd[1]);
				break;
				case 'C':
				case 'c':
					oper(follow_case(cmd[0],'b'),cmd[1],cmd[2]);
					oper(follow_case(cmd[0],'b'),cmd[3],cmd[4]);
					oper(follow_case(cmd[0],'c'),cmd[5],cmd[6]);
				break;
				case 'S':
				case 's':
					if( j>0 && (res[j-1][0]==='C'||res[j-1][0]==='c'||res[j-1][0]==='S'||res[j-1][0]==='s'))
					{
						var x = res[j-1][res[j-1].length-2],
							y = res[j-1][res[j-1].length-1],
							x2= res[j-1][res[j-1].length-4],
							y2= res[j-1][res[j-1].length-3];
						oper('b',x-x2,y-y2);
					}
					else
						oper('b',0,0);
					oper(follow_case(cmd[0],'b'),cmd[1],cmd[2]);
					oper(follow_case(cmd[0],'c'),cmd[3],cmd[4]);
				break;
				case 'Q':
				case 'q':
					oper(follow_case(cmd[0],'b'),cmd[1],cmd[2]);
					oper(follow_case(cmd[0],'c'),cmd[3],cmd[4]);
				break;
				case 'T':
				case 't':
					if( j>0 && (res[j-1][0]==='Q'||res[j-1][0]==='q'||res[j-1][0]==='T'||res[j-1][0]==='t'))
					{
						if( output.length < 2 || 
							is_upper(output[output.length-1][0])!==is_upper(output[output.length-2][0]) )
						{
							log('unhandled svg path command '+cmd[0]);
							break;
						}
						var x = output[output.length-1][1],
							y = output[output.length-1][2],
							x2= output[output.length-2][1],
							y2= output[output.length-2][2];
						output.push(['b',x-x2,y-y2]);
					}
					else
						oper('b',0,0);
					oper(follow_case(cmd[0],'c'),cmd[1],cmd[2]);
				break;
				case 'A':
				case 'a':
					log('unhandled svg path command '+cmd[0]);
					oper(follow_case(cmd[0],'l'),cmd[cmd.length-2],cmd[cmd.length-1]);
				break;
			}
			switch (cmd[0])
			{
				case 'H': lastpos.x = cmd[1];  break;
				case 'h': lastpos.x += cmd[1]; break;
				case 'V': lastpos.y = cmd[1];  break;
				case 'v': lastpos.y += cmd[1]; break;
				case 'Z': case 'z':
					lastpos.x = lastM.x;
					lastpos.y = lastM.y;
				break;
				case 'M': case 'm':
				case 'L': case 'l':
				case 'C': case 'c':
				case 'S': case 's':
				case 'Q': case 'q':
				case 'T': case 't':
				case 'A': case 'a':
					if( is_upper(cmd[0]))
					{
						lastpos.x = cmd[cmd.length-2];
						lastpos.y = cmd[cmd.length-1];
					}
					else
					{
						lastpos.x += cmd[cmd.length-2];
						lastpos.y += cmd[cmd.length-1];
					}
				break;
			}
			switch (cmd[0])
			{
				case 'M': case 'm':
					lastM.x = lastpos.x;
					lastM.y = lastpos.y;
				break;
			}
		}
	}
	console.log(output);
	printer.print(output,svgdoc.filename);
	/* //preview
	var pre="<div><svg>";
	pre+="<path fill='none' stroke='black' stroke-width='5' d='M0,0 L3000,0 L3000,2500 L0,2500 L0,0 ";
	for( var i=0; i<output.length; i++)
	{
		if( output[i][0]==='M')
			pre+='M';
		else if( is_upper(output[i][0]))
			pre+='L';
		else
			pre+='l';
		pre+= (-output[i][2])+' '+output[i][1];
	}
	pre+="' /></svg></div>";
	$('svg_inner_2').innerHTML += pre;
	svg_panzoom($('svg_inner_2').firstChild);
	console.log(JSON.stringify(output)); */
	function follow_case(ref,tar)
	{
		if( ref===ref.toUpperCase())
			return tar.toUpperCase();
		else
			return tar.toLowerCase();
	}
	function is_upper(a)
	{
		return a===a.toUpperCase();
	}
	function trans(x,y) //coordinate transform
	{
		var pos = svgdoc.createSVGPoint();
		pos.x = x;
		pos.y = y;
		//element to svg space
		pos = pos.matrixTransform(pathm);
		//svg to screen space
		pos = pos.matrixTransform(ctm);
		//screen to reference svg space
		pos = pos.matrixTransform(refm);
		//reference svg to paper space
		pos = pos.matrixTransform(pprm);
		/* //screen to view space
		pos.x -= par.x;
		pos.y -= par.y;
		//view to print space
		pos.x = pos.x*(pr.w/par.w);
		pos.y = pos.y*(pr.w/par.w);
		var yy = pos.y;
		pos.y = pr.w-pos.x;
		pos.x = yy;*/
		return {x:pos.x,y:pos.y};
	}
	function oper(O,x,y)
	{
		var pp = trans(x,y);
		if( O===O.toUpperCase()) //absolute coordinate
			output.push([O,pp.x,pp.y]);
		else //relative
			output.push([O,pp.x-po.x,pp.y-po.y]);
	}
}

});
