define(function()
{
	//tabs
	var def = 0; //default page
	var pages = document.getElementsByClassName('page');
	var btn = $('navbar').getElementsByTagName('button');
	if( btn && btn.length)
		for( var i=0; i<btn.length; i++)
		{
			btn[i].onclick = (function(j)
			{
				return function()
				{
					showpage(j);
				}
			}(i));
			if( pages[i].getAttribute('default')!==null)
				def = i;
		}
	showpage(def);
	function showpage(num)
	{
		for( var i=0; i<pages.length; i++)
			pages[i].style.display='none';
		$('page'+num).style.display='';
	}
});
