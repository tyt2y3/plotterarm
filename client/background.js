chrome.app.runtime.onLaunched.addListener(function() {
	chrome.app.window.create('main.html',
		{
		'width': 420,
		'height': 600
		}, function(win)
		{
		});
});
