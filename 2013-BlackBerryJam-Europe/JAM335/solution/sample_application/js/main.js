var meta = document.createElement("meta");
meta.setAttribute('name','viewport');
meta.setAttribute('content','initial-scale='+ (1/window.devicePixelRatio) + ',user-scalable=no');
document.getElementsByTagName('head')[0].appendChild(meta);

var webworksreadyFired = false;
document.addEventListener('webworksready', function(e) {
	// This is code to ensure that if webworksready is fired multiple times we still only init() one time
	if (webworksreadyFired) return;
	webworksreadyFired = true;
	bb.init({
		controlsDark: false,
		listsDark: false,
		bb10ForPlayBook: true,
		onscreenready: function(element, id, params) {
			// Before insert
		},
		ondomready: function(element, id, params) {
			// After insert
			if (id == 'led_options' && blackberry_led) {
				blackberry_led.init(element);
			}
		}
	});
	bb.pushScreen('led_options.html', 'led_options');
}, false);

// Fire the webworksready event for PlayBook and BBOS
window.addEventListener('load',function() {
	if (navigator.userAgent.indexOf('Version/10.0') < 0) {
		var evt = document.createEvent('Events');
		evt.initEvent('webworksready', true, true);
		document.dispatchEvent(evt);
	}
});