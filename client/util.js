/*\
 * util
 * javascript utilities
\*/
define(function(){

var F={

// javascript-----------------

/**	inject a .js file
	deprecated in favour of the use of head.js, now requirejs
*/
/* js: function (filename)
{
	var head = document.getElementsByTagName('head')[0];
	var script = document.createElement('script');
	script.src = filename;
	script.type = 'text/javascript';
	head.appendChild(script);
}, */

/*\
 * util.css
 * attach a stylesheet to page
 [ method ]
 - filename (string)
\*/
css: function (filename)
{
	var head = document.getElementsByTagName('head')[0];
	var link = document.createElement('link');
	link.href = filename;
	link.rel = 'stylesheet';
	link.type = 'text/css';
	head.appendChild(link);
},

/**
 * util.double_delegate
 * double delegate a function
 [ method ]
 * [reference](http://roberthahn.ca/articles/2007/02/02/how-to-use-window-onload-the-right-way/)
 */
double_delegate: function (function1, function2)
{
	return function() {
	if (function1)
		function1.apply(this,Array.prototype.slice.call(arguments));
	if (function2)
		function2.apply(this,Array.prototype.slice.call(arguments));
	}
},

/*\
 * util.make_array
 [ method ]
 - target (any)
 * if target is:
 - (array) returns target as is.
 - (object) returns object encapsulated in an array.
 - (falsy) (null, undefined or zero), returns an empty array.
\*/
make_array: function (target)
{
	if( target)
	{
		if( target instanceof Array)
			return target;
		else
			return [target];
	}
	else
		return [];
},

//data structure------------

/*\
 * util.arr_search
 [ method ]
 - arr (array) target to be searched
 - fc_criteria (function) return true when an accepted element is passed in
 - [fc_replace] (function) to return a replacement value when original value is passed in
 - [search_all] (boolean) if true, will search through entire array before returning the list of indices, otherwise, will return immediately at the first accepted element
 = (array) if `search_all` is true
 = (object) if `search_all` if false
\*/
arr_search: function ( arr,
		fc_criteria,
		fc_replace,
		search_all)
{
	var found_list=new Array();
	//for ( var i=0; i<arr.length; i++)
	for ( var i in arr)
	{
		if ( fc_criteria(arr[i],i))
		{
			if ( fc_replace) {
				arr[i] = fc_replace(arr[i]);
			}
			if ( !search_all) {
				return i;
			} else {
				found_list.push(i);
			}
		}
	}
	if ( search_all) {
		return found_list;
	} else {
		return -1;
	}
},

/*\
 * util.push_unique
 * push only if not existed in array
 [ method ]
 - array (array)
 - element (object)
 = (boolean) true if added
\*/
push_unique: function ( array, element)
{
	var res = F.arr_search( array, function(E){return E==element} );
	if (res == -1)
	{
		array.push(element);
		return true;
	}
},

/*\
 * util.extend_object
 * extend obj1 with all members of obj2
 [ method ]
 - obj1, obj2 (object)
 = (object) a modified obj1
\*/
extend_object: function (obj1, obj2)
{
	for (var p in obj2)
	{
		if ( typeof obj2[p]=='object' )
		{
			obj1[p] = arguments.callee((obj1[p]?obj1[p]:{}), obj2[p]);
		} else
		{
			obj1[p] = obj2[p];
		}
	}
	return obj1;
	// http://stackoverflow.com/questions/122102/what-is-the-most-efficient-way-to-clone-a-javascript-object
},

/*\
 * util.to_text
 * convert an object into JSON text
 * 
 * most of the time you should use built-in `JSON.stringify` instead
 [ method ]
 - obj (object)
 - name (string) the object's name
 - [sep] (string) separator, default as `\n`
 - [pretext] (string) used in recursion only, set it to null
 - [filter] (function) a filter `function(p,P)` passing in name p and object P, return 1 to hide the attribute, OR return a string to be shown
 - [TTL] (number) time-to-live to prevent infinite looping
|	var obj={};
|	obj.a={};
|	obj.a.x=1;
|	obj.a.y=1;
|	obj.b='hello';
|	obj.c=12;
|	console.log(util.to_text(obj,'obj'));
|	//outputs:
|	obj:
|	{
|		a:
|		{
|			'x': 1,
|			'y': 1
|		},
|		'b': 'hello',
|		'c': 12
|	}
\*/
to_text: function (
	obj2, name,
	sep,
	pretext,
	filter,
	TTL
)
{
	if( TTL===0) return '';
	if( !TTL) TTL=30;
	if( !sep) sep='\n';
	if( !pretext) pretext='';

	var str = pretext+ name +':'+sep;
	str+= pretext+ '{';
	var cc=0;
	for (var p in obj2)
	{
		var fil = filter && filter(p,obj2[p]);
		if( fil==1)
		{
			//do nothing
		}
		else if( typeof fil=='string')
		{
			str += (cc?',':'')+sep+pretext+'\t'+"'"+p+"'"+': '+fil;
		}
		else
		{
			if( obj2[p].constructor==Object )
			{
				str += (cc?',':'')+sep+arguments.callee(obj2[p],p,sep,pretext+'\t',filter,TTL-1);
			} else
			{
				str += (cc?',':'')+sep+pretext+'\t'+"'"+p+"'"+': ';
				if( typeof obj2[p]=='string')
					str += "'";
				str += obj2[p];
				if( typeof obj2[p]=='string')
					str += "'";
			}
		}
		cc=1;
	}
	str+= sep+pretext+ '}';
	return str;
},

/*\
 * util.extract_array
 [ method ]
 * extract properties from an array of objects
|	//say we have
|	[ {x:x1,y:y1}, {x:x2,y:y2}, {x:x3,y:y3},,,]
|	//we want to extract it into
|	{
|	  x:
|		[ x1, x2, x3,,, ],
|	  y:
|		[ y1, y2, y3,,, ]
|	}
 - array (array)
 - prop (string) property name
 * or
 - prop (array) array of property name
 = (array) extracted array
\*/
extract_array: function(array, prop)
{
	var out={};
	prop = F.make_array(prop);

	for( var j in prop)
		out[prop[j]] = [];

	for( var i=0; i<array.length; i++)
	{
		for( var k=0; k<prop.length; k++)
		{
			var P=prop[k];
			out[P].push(array[i][P]);
		}
	}
	return out;
}

};

return F;
});
