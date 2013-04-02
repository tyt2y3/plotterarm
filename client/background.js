chrome.app.runtime.onLaunched.addListener(function() {
	chrome.app.window.create('main.html',
		{
		'width': 425,
		'height': 600,
		'frame': 'chrome'
		}, function(win)
		{
		});
});
